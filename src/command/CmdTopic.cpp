/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macoulib <macoulib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:18:22 by macoulib          #+#    #+#             */
/*   Updated: 2026/02/17 14:20:54 by macoulib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// CmdTopic.cpp

#include <iostream>
#include "CommandMgr.hpp"

void CommandMgr::cmdTopic(Server &s, Client *c, const Command &cmd)
{
	if (!c->getAuthenticated())
	{
		s.clientCast(c->getFd(), Utils::replace(c, std::string(S_451), "", "") + S_RN);
		return ;
	}
    if (cmd.params.size() < 1) {
        std::string msg = ":ft_irc 461 " + c->getNickName() + " TOPIC :Not enough parameters\r\n";
		s.clientCast(c->getFd(), msg);
        return;
    }
	if (!cmd.params.empty())
	{
		std::string channelName = cmd.params[0];
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		if (!s.hasChannel(channelName))
		{
			std::string msg = ": ft_irc 403 " + c->getNickName() + " " + channelName + " :No such channel\r\n";
			s.clientCast(c->getFd(), msg);
			return;
		}
		Channel &channel = s.getChannel(channelName);
		// ---  lecture du topic ---
		if (cmd.params.size() == 1 && !cmd.hasTrailing) {
			if (channel.getTopic().empty()) {
				std::string msg = ": ft_irc 331 " + c->getNickName() + " " + channelName + " :No topic is set\r\n";
				s.clientCast(c->getFd(), msg);
			} else {
				std::string msg = ": ft_irc 332 " + c->getNickName() + " " + channelName + " :" + channel.getTopic() + "\r\n";
				s.clientCast(c->getFd(), msg);
			}
			return;
		}
		// ---  modify topic ---
		std::string newTopic;
		if (cmd.hasTrailing)
		{
			newTopic = cmd.trailing;
		}
		else
		{
			newTopic = cmd.params[1];
			if (!newTopic.empty() && newTopic[0] == ':')
				newTopic.erase(0, 1);
		}
		//  check mode +t (only operator can write)
		if (channel.hasModeT() && !channel.isOperator(c)) {
			std::string msg = ": ft_irc 482 " + c->getNickName() + " " + channelName + " :You're not channel operator\r\n";
			s.clientCast(c->getFd(), msg);
			return;
		}
		channel.setTopic(newTopic);
		std::string msg = ":" + c->getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
		channel.broadcast(s, msg);


	}
}
