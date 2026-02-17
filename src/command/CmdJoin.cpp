/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macoulib <macoulib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:06:44 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/17 13:57:12 by macoulib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// CmdJoin.cpp

#include "CommandMgr.hpp"
#include "Parser.hpp"
#include "iostream"

static void sendNames(Server &s, Client *c, Channel *channel)
{
	const std::map<int, Client *> &cs = channel->getClients();
	for (std::map<int, Client *>::const_iterator it = cs.begin(); it != cs.end(); it++)
	{
		std::string nickName = it->second->getNickName();
		if (channel->isOperator(it->second))
			nickName = C_OPERATOR + nickName;
		std::string mes = Utils::replace(c, S_353, B_NAMES, it->second->getNickName());
		mes = Utils::replace(c, mes, B_CHANNEL, channel->getName());
		s.clientCast(c->getFd(), mes + S_RN);
	}
	std::string mes = Utils::replace(c, S_366, B_NICK, c->getNickName());
	s.clientCast(c->getFd(), Utils::replace(c, mes, B_CHANNEL, channel->getName()) + S_RN);
}

void CommandMgr::cmdJoin(Server &s, Client *c, const Command &cmd)
{
	if (!c->getAuthenticated())
	{
		s.clientCast(c->getFd(), Utils::replace(c, std::string(S_451), "", "") + S_RN);
		return ;
	}
	if (cmd.params.size() < 1 || cmd.params[0].empty())
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_JOIN) + S_RN);
		return ;
	}
	std::vector<std::string> channels = Parser::parseJoin(cmd.params[0]);

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channel_name = channels[i];
		if (channel_name[0] != '#')
			channel_name = std::string("#") +  channel_name;
		Channel *channel;
		if (s.hasChannel(channel_name))  // join
		{
			channel = &s.getChannel(channel_name);
			if (channel->hasClient(c))
				continue;
		}
		else
		{
			channel = &s.addChannel(channel_name);
			channel->addOperator(c->getFd());
		}
		if (channel->isInviteOnly() && !channel->isInvited(c)) {
    		s.clientCast(c->getFd(), ":ft_irc 473 " + channel->getName() + " :Cannot join channel (+i)\r\n");
    		continue;
		}
		if ((channel->getLimitUser() != 0) && (channel->getLimitUser() <= channel->getClients().size())) {
    		s.clientCast(c->getFd(), ":ft_irc 471 " + channel->getName() + " :Cannot join channel (+l)\r\n");
    		continue;
		}
		if (channel->hasKey())
		{
			std::string key = "";
			if (cmd.params.size() > 1)
				key = cmd.params[1];
			if (channel->getKey() != key)
			{
				s.clientCast(c->getFd(), Utils::replace(c, S_475, B_CHANNEL, channel->getName()) + S_RN);
				continue;
			}
		}
		if (channel->getTopic() == "")
		{
			s.clientCast(c->getFd(), Utils::replace(c, S_331, B_CHANNEL, channel_name) + S_RN);
		}
		else
		{
			std::string str = Utils::replace(c, S_332, B_CHANNEL, channel_name);
			str = Utils::replace(c, str, B_TOPIC, channel->getTopic());
			s.clientCast(c->getFd(), str + S_RN + '\n');
		}
		channel->addClient(c);
		std::string str = Utils::replace(c, S_MESSAGE_JOIN, B_NICK, c->getNickName());
		str = Utils::replace(c, str, B_USER, c->getUserName());
		str = Utils::replace(c, str, B_CHANNEL, channel_name);
		s.clientsCast(channel->getClients(), str + S_RN);
		sendNames(s, c, channel);
	}
}

