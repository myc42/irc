/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:00:45 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/13 16:21:32 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "constants.h"
#include "Parser.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <iostream>

void Server::clean()
{
	_isRunning = false;
	_fd = Utils::ft_close(_fd);
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Utils::ft_close(it->first);
		delete it->second;
	}
	_clients.clear();
	_pollFds.clear();
}

Client *Server::getClient(int fd)
{
	std::map<int, Client *>::iterator it  = _clients.find(fd);
	if (it == _clients.end())
		return NULL;
	return it->second;
}

Client *Server::getClient(const std::string &nickName)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second && it->second->getNickName() == nickName)
			return it->second;
	}
	return NULL;
}

void Server::setIsRunning(bool b)
{
	_isRunning = b;
}

bool Server::hasChannel(const std::string &s)
{
	std::string key = Parser::toUpper(s);
	std::map<std::string, Channel>::iterator it  = _channels.find(key);
	if (it == _channels.end())
		return false;
	return true;
}

Channel &Server::getChannel(const std::string &s)
{
	std::string key = Parser::toUpper(s);
	std::map<std::string, Channel>::iterator it  = _channels.find(key);
	if (it == _channels.end())
		throw std::runtime_error(S_ERR_CHANNEL);
	return it->second;
}

void Server::setPwd(const std::string &pwd) {_pwd = pwd; }
const std::string &Server::getPwd() const { return _pwd; }

Channel &Server::addChannel(const std::string &s)
{
	std::string key = Parser::toUpper(s);
	if (_channels.find(key) != _channels.end())
		throw std::runtime_error(S_ERR_CHANNEL_EXISTS);
	_channels[key] = Channel(key);
	return _channels[key];
}

void Server::printChannel(int fd, Server &s) const
{
	for (std::map<std::string, Channel>::const_iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		s.clientCast(fd, "name=[" +  it->first + "]");
		s.clientCast(fd, " topic=[" + it->second.getTopic() + "]");
	s.clientCast(fd, S_RN);
	}
}

const std::string &Server::getStartTime() const { return _startTime; }
const std::map<int, Client*> &Server::getClients() const { return _clients; }
const std::map<std::string, Channel> &Server::getChannels() const { return _channels; }
void Server::deleteChannel(const std::string &s) { _channels.erase(Parser::toUpper(s)); }
bool Server::getExtra() { return _extra;}
void Server::addClient(Client *client) {_clients[client->getFd()] = client;}

void Server::clientQuit(int fd)
{
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return;
	for (std::map<std::string, Channel>::iterator it2 = _channels.begin(); it2 != _channels.end();)
	{
		Channel &channel = it2->second;
		channel.removeClient(it->second->getFd());
		if (channel.getClients().empty())
			_channels.erase(it2++);
		else
			it2++;
	}
	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (fd == _pollFds[i].fd)
		{
			Utils::ft_close(fd);
			delete it->second;
			_clients.erase(it);
			_pollFds[i] = _pollFds.back();
			_pollFds.pop_back();
			break;
		}
	}
}
