/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 15:45:03 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/06 15:49:29 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include "Channel.hpp"
#include "CommandMgr.hpp"
#include "Command.hpp"

Channel::Channel() {};

Channel::Channel(const std::string &name):_name(name), _topic(""), _pwd(""), _clients(), _operators(), _invited(), _modeI(false), _modeT(false), _modeK(false), _limit(0) {}

Channel::Channel(const Channel &other): _name(other._name), _topic(other._topic), _pwd(other._pwd), _clients(other._clients), _operators(other._operators), _invited(other._invited), _modeI(other._modeI), _modeT(other._modeT), _modeK(other._modeK), _limit(other._limit) {}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		_name = other._name;
		_topic = other._topic;
		_pwd = other._pwd;
		_clients = other._clients; // shallow copy
		_operators = other._operators;
		_invited = other._invited;
		_modeI = other._modeI;
		_modeT = other._modeT;
		_modeK = other._modeK;
		_limit = other._limit;
	}
	return *this;
}


Channel::~Channel() {}

void Channel::addClient(Client *c)
{
	_clients[c->getFd()] = c;
}

void Channel::removeClient(int fd) {
	_clients.erase(fd);
	_operators.erase(fd);
	_invited.erase(fd);
}

void Channel::addOperator(int fd)
{

	_operators.insert(fd);
}

const std::map<int , Client*> &Channel::getClients() const { return _clients; }
const std::string &Channel::getName() const { return _name; }
const std::string &Channel::getTopic() const { return _topic;}
void Channel::setTopic(const std::string &topic) { _topic = topic ;}

Client *Channel::getClient(const std::string &s)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second && it->second->getNickName() == s)
			return it->second;
	}
	return NULL;
}

bool Channel::hasModeT() const { return _modeT; }

bool Channel::isOperator(Client* c) const
{
	if (!c) return false;
	return _operators.find(c->getFd()) != _operators.end();
}

void Channel::broadcast(Server &s, const std::string &msg)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* c = it->second;
		if (c)
		{
			s.clientCast(c->getFd(), msg);
		}
	}
}

bool Channel::hasClient(Client *c)
{
	if (c == NULL)
		return false;
    return _clients.find(c->getFd()) != _clients.end();
}

bool Channel::hasKey() const
{
    return _modeK;
}

void Channel::setKey(const std::string &key)
{
	_pwd = key;
	_modeK = true;
}

const std::string &Channel::getKey() const { return _pwd; }

void Channel::removeKey()
{
	_pwd.clear();
	_modeK = false;
}

void Channel::setInviteOnly(bool value)
{
    _modeI = value;
}

void Channel::setModeT(bool value)
{
    _modeT = value;
}

void Channel::removeOperator(Client *c)
{
    if (c == NULL) return;
    _operators.erase(c->getFd());
}

void Channel::addInvite(Client *c)
{
	if (c == NULL)return ;
    _invited.insert(c->getFd());
}

bool Channel::isInvited(Client *c) const
{
	if (c == NULL) return false;
    return _invited.find(c->getFd()) != _invited.end();
}

void Channel::removeInvite(Client *c)
{
	if (c == NULL) return;
    _invited.erase(c->getFd());
}

void Channel::setLimitUser(int limit)
{
	if (limit < 0)
		return;
	_limit = limit;
}

void Channel::removeLimitUser()
{
	_limit = 0;
}

unsigned long Channel::getLimitUser() const { return _limit; }

bool Channel::hasInvite() const { return _modeI; }

std::string Channel::getMode()
{
	std::string s = "";
	if (hasInvite() || hasModeT() || hasKey() || getLimitUser() > 0)
		s += '+';
	if (hasInvite())
		s += 'i';
	if (hasModeT())
		s += 't';
	if (hasKey())
		s += 'k';
	if (getLimitUser() > 0)
	{
		std::ostringstream ost;
		ost << getLimitUser();
		s += "l " + ost.str();
	}
	return s;
}