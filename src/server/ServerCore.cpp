/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 15:53:53 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/13 16:56:28 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "constants.h"
#include "Utils.hpp"
#include "SignalMgr.hpp"
#include "CommandMgr.hpp"
#include "Bot.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>

Server::Server(std::string port, const std::string pwd, bool extra):_fd(M1), _port(port), _pwd(pwd), _isRunning(false), _clients(), _channels(), _pollFds(), _extra(extra)
{
	_startTime = Utils::getDate();
}

Server::~Server()
{
	clean();
}

void Server::addPoll(int fd, short events)
{
	struct pollfd p;
	p.fd = fd;
	p.events = events;
	p.revents = 0;
	_pollFds.push_back(p);

}

void Server::initSocket()
{
	struct addrinfo hints;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE; // binds all IP
	hints.ai_family = AF_UNSPEC; // both IPv4, IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_protocol = IPPROTO_IP;
	struct addrinfo *ret = NULL;
	int info = getaddrinfo(NULL, _port.c_str(), &hints, &ret);
	if (info != 0) throw std::runtime_error(std::string(S_GETADDRINFO));
	for (struct addrinfo *ai = ret; ai	 != NULL; ai = ai->ai_next)
	{
		_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (_fd < 0) continue;
		int optval = 1;
		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) // retry if close
		{
			_fd = Utils::ft_close(_fd);
			continue;
		}
		if (bind(_fd, ai->ai_addr, ai->ai_addrlen) == 0)
			break;
		_fd = Utils::ft_close(_fd);
	}
	freeaddrinfo(ret); // leaks if not
	if (_fd < 0) throw std::runtime_error(S_ERROR_SOCKET);
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) throw std::runtime_error(S_ERROR_FCNTL);
	if (listen(_fd, SOMAXCONN) < 0) throw std::runtime_error(S_ERROR_LISTEN);
	addPoll(_fd, POLLIN);
}

void Server::broadcast(int fdBroadcast, const std::string &s)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first != fdBroadcast)
		{
			it->second->Out() += s;
			setPollOut(it->first);
		}
	}
}

void Server::clientCast(int fd, const std::string &s)
{
	Client *c = getClient(fd);
	if (c != NULL)
	{
		c->Out() += s;
		setPollOut(fd);
	}
}

void Server::clientsCast(const std::map<int, Client *> &clients, const std::string &s)
{
	for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (!it->second) continue;
		it->second->Out() += s;
		setPollOut(it->first);
	}
}

void Server::setPollOut(int fd)
{
	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (fd == _pollFds[i].fd)
		{
			_pollFds[i].events |= POLLOUT;
			return;
		}
	}
}


void Server::acceptNewClient()
{
	while (true)
	{
		struct sockaddr_storage client_addr;
		socklen_t size = sizeof(client_addr);
		int fd = accept(_fd, (struct sockaddr *)&client_addr, &size);
		if (fd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return;
			if (errno == EINTR)
				continue;
			return;
		}
		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		{
			fd = Utils::ft_close(fd);
			throw std::runtime_error(S_ERROR_FCNTL);
		}
		Client *c = new Client(fd);
		_clients[fd] = c;
		c->setHost(S_UNKNOWN);
		if (client_addr.ss_family == AF_INET)
		{
			struct sockaddr_in *a_sockaddr = (struct sockaddr_in *)&client_addr;
			char *ip = inet_ntoa(a_sockaddr->sin_addr);
			if (ip != NULL)
				c->setHost(std::string(ip));
		}
		addPoll(fd, POLLIN);
		c->Out() += S_CLIENT_CONNECTED;
		setPollOut(fd);
		std::cout << S_CLIENT_FD << fd << std::endl;
	}
}

void Server::readClient(size_t index)
{
	int fd = _pollFds[index].fd;
	Client *c = getClient(fd);
	char buffer[MAX_BUFFER];
	ssize_t n = recv(fd, buffer, sizeof(buffer), 0); // max is 512 in RFC1459
	if (n == 0)
	{
		clientDisconnect(index, S_CLIENT_CLOSE);
		return;
	}
	if (n < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			return;
		clientDisconnect(index, S_CLIENT_CLOSE_RECV);
		return;
	}
	c->In().append(buffer, n);
	Utils::removeCR(c->In());
	while (true)
	{
		c = getClient(fd);
		if (c == NULL) return ;
		size_t pos = c->In().find(C_N);
		if (pos == std::string::npos) break;
		std::string line = c->In().substr(0, pos);
		c->In().erase(0, pos + 1);
		CommandMgr::cmd(*this, c, line);
	}
	c = getClient(fd);
	if (c == NULL) return ;
	if (c->In().size() > MAX_BUFFER)
	{
		// c->In().erase(0, c->In().size() - MAX_BUFFER);
		c->In().clear();
		std::cout << S_BUFFER_OVERFLOW << std::endl;
	}
}

void Server::writeClient(size_t index)
{
	int fd = _pollFds[index].fd;
	Client *c = getClient(fd);
	if (c->Out().empty())
	{
		_pollFds[index].events &= ~ POLLOUT;
		return;
	}
	ssize_t n = send(fd, c->Out().c_str(), c->Out().size(), MSG_NOSIGNAL);
	if (n < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK  || errno == EINTR)
			return;
		clientDisconnect(index, S_CLIENT_CLOSE_SEND);
		return;
	}
	c->Out().erase(0, n);
	if (c->Out().empty())
		_pollFds[index].events &= ~ POLLOUT;

}

void Server::clientDisconnect(size_t index, const std::string &s)
{
	int fd = _pollFds[index].fd;
	Client *c = getClient(fd);
	std::cout << S_DISCONNECT << fd << S_MESSAGE << s.c_str() << std::endl;
	if (c == NULL)
	{
		fd = Utils::ft_close(fd);
		_pollFds[index] = _pollFds.back();
		_pollFds.pop_back();
		return ;
	}
	std::string quitMsg = ":" + c->getNickName() + "!" + c->getUserName() + "@localhost QUIT : Disconnection" + S_RN;
    broadcast(M1, quitMsg);
	clientQuit(c->getFd());
}

void Server::run()
{
	SignalMgr::setupSignal();
	initSocket();
	std::cout << "Socket : Port[" << _port << "]" << std::endl;
	_isRunning = true;
	if (_extra)
		Bot::createBot(*this);
	while (_isRunning)
	{
		if (SignalMgr::handleSignal()) break;
		int timeOut = 1000;
		if (_pollFds.empty()) continue;
		int p = poll(&_pollFds[0], _pollFds.size(), timeOut);
		if (p < 0)
		{
			if (errno == EINTR)
				continue;
			std::cerr << S_ERROR_POLL << std::endl;
			break;
		}
		if (p == 0)
		{
			continue;
		}
		for (size_t i = 0; i < _pollFds.size();)
		{
			//std::cout << "IN/OUT=" << i << ":pfd=" << _pollFds[i].fd << ":fd=" << _fd << ":size=" <<  _pollFds.size() << ":p=" << p << std::endl;
			int fdi = _pollFds[i].fd;
			short events = _pollFds[i].revents;
			if (events == 0)
			{
				i++;
				continue;
			}
			if (fdi == _fd)
			{
				if (events & POLLIN)
					acceptNewClient();
				_pollFds[i].revents = 0;
				i++;
				continue;
			}
			if (events & (POLLHUP | POLLERR | POLLNVAL))
			{
				clientDisconnect(i, "DISCO");
					continue;
			}
			if (events & POLLIN)
			{
				//std::cout << "IN" << std::endl;
				readClient(i);
				if (i >= _pollFds.size() || fdi != _pollFds[i].fd) // A client was disco
					continue;
			}
			if (events & POLLOUT)
			{
				//std::cout << "OUT" << std::endl;
				writeClient(i);
				if (i >= _pollFds.size() || fdi != _pollFds[i].fd) // A client was disco
				continue;
			}


			_pollFds[i].revents = 0;
			i++;
		}
	}
}
