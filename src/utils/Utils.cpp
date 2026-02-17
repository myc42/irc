/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:19:40 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/11 20:07:08 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include "Utils.hpp"
#include "constants.h"

bool Utils::isAllDigits(const std::string &s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); i++)
		if(! std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	return true;
}

void Utils::removeCR(std::string &s)
{
	size_t j = 0;
	for (size_t i = 0; i < s.size(); i++)
	{
		if (s[i] != C_R)
			s[j++] = s[i];
	}
	s.resize(j);
}

std::string Utils::replace(std::string s, const std::string &sOld, const std::string &sNew)
{
	size_t pos = 0;
	while ((pos = s.find(sOld, pos))!= std::string::npos)
	{
		s.replace(pos, sOld.length(), sNew);
		pos += sNew.length();
	}
	return s;
}

std::string Utils::replace(Client *c, std::string s, const std::string &sOld, const std::string &sNew)
{
	s = Utils::replace(s, B_SERVER_NAME, S_SERVER_NAME);
	if (c != NULL)
		s = Utils::replace(s, B_NICK, c->getNickName());
	if (sOld.empty() /*|| sNew.empty()*/)
		return s;
	s = Utils::replace(s, sOld, sNew);
	return s;
}

bool Utils::isValidNickname(const std::string& nick)
{
	if (nick.empty() || nick.length() > 9)
		return false;
	// The first character: letter or underscore
	if (!std::isalpha(nick[0]) && nick[0] != '_')
		return false;
	// Character verification
	for (size_t i = 0; i < nick.length(); i++)
	{
		if (!std::isalnum(nick[i]) && nick[i] != '_')
			return false;
	}
	return true;
}

void Utils::welcome(Server &s, Client *c)
{
	std::string str;
	str = Utils::replace(c, S_001, B_SERVER_NAME, S_SERVER_NAME);
	str = Utils::replace(c, str, B_NICK, c->getNickName());
	str = Utils::replace(c, str, B_USER, c->getUserName());
	str = Utils::replace(c, str, B_HOST, c->getHost());
	s.clientCast(c->getFd(), str + S_RN);
	str = Utils::replace(c, S_002, B_NICK, c->getNickName());
	str = Utils::replace(c, str, B_VERSION, S_VERSION);
	str = Utils::replace(c, str, B_SERVER_NAME, S_SERVER_NAME);
	s.clientCast(c->getFd(), str + S_RN);
	str = Utils::replace(c, S_003, B_DATE, s.getStartTime());
	str = Utils::replace(c, str, B_SERVER_NAME, S_SERVER_NAME);
	str = Utils::replace(c, str, B_NICK, c->getNickName());
	s.clientCast(c->getFd(), str + S_RN);
	str = Utils::replace(c, S_004, B_NICK, c->getNickName());
	str = Utils::replace(c, str, B_SERVER_NAME, S_SERVER_NAME);
	str = Utils::replace(c, str, B_VERSION, S_VERSION);
	str = Utils::replace(c, str, B_USER_MODE, S_USER_MODE);
	str = Utils::replace(c, str, B_CHANNEL_MODE, S_CHANNEL_MODE);
	s.clientCast(c->getFd(), str + S_RN);
}

std::string Utils::getDate()
{
	std::time_t aTime = std::time(NULL);
	std::tm *tm = std::localtime(&aTime);
	char buffer[256];
	std::strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", tm);
	return std::string(buffer);
}

void Utils::printVar(const std::string &s, int n)
{
	std::cout << s << "=[" << n << "]" << std::endl;
}

void Utils::printVar(const std::string &s1, const std::string &s2)
{
	std::cout << s1 << "=[" << s2 << "]" << std::endl;
}

void Utils::printVar(const std::string &s1, const std::vector<std::string> &tab)
{
	for (size_t i = 0; i < tab.size(); i ++)
		std::cout << s1 << "[" << i << "]=[" << tab[i] << "]" << std::endl;
}

int Utils::ft_close(int fd)
{
	if (fd != M1)
		close(fd);
	return M1;
}

std::string Utils::prefix(Client *c)
{
	std::string prefix = ":" + c->getNickName() + "!" + c->getUserName() + "@" + c->getHost();
	return prefix;
}
