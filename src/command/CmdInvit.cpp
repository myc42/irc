/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdInvit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:15:55 by macoulib          #+#    #+#             */
/*   Updated: 2026/02/09 17:20:18 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "CommandMgr.hpp"
#include "Parser.hpp"


 void CommandMgr::cmdInvite(Server &s, Client *c, const Command &cmd)
{
    if (!c->getAuthenticated()) {
        s.clientCast(c->getFd(), Utils::replace(c , std::string(S_451), "", "") + S_RN);
        return;
    }
    if (cmd.params.size() < 2)
	{
        s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_INVITE) + S_RN);
        return;
    }

    std::string nick = cmd.params[0];
    std::string channelName = cmd.params[1];

	if (channelName.empty())
		return s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_INVITE) + S_RN);
    if (channelName[0] != '#') channelName = "#" + channelName;
    if (!s.hasChannel(channelName)) {
        s.clientCast(c->getFd(), Utils::replace(c, S_403, B_S1, channelName) + S_RN);
        return;
    }

    Channel &channel = s.getChannel(channelName);
    Client *client = s.getClient(nick);

    if (!client) {
        s.clientCast(c->getFd(), ":ft_irc 401 " + nick + " :No such nick\r\n");
        return;
    }

    if (!channel.hasClient(c)) {
        s.clientCast(c->getFd(), ":ft_irc 442 " + c->getNickName() + " " + channelName + " :You're not on that channel\r\n");
        return;
    }
    if (channel.isInviteOnly() && !channel.isOperator(c)) {
        s.clientCast(c->getFd(), ":ft_irc 482 " + c->getNickName() + " " + channelName + " :You're not channel operator\r\n");
        return;
    }

    if (channel.hasClient(client)) {
        s.clientCast(c->getFd(), ":ft_irc 443 " + nick + " " + channelName + " :is already on channel\r\n");
        return;
    }
    channel.addInvite(client);
    std::string msg = ":" + c->getNickName() + " INVITE " + nick + " :" + channelName + "\r\n";
    s.clientCast(client->getFd(), msg);
    s.clientCast(c->getFd(), ":ft_irc 341 " + c->getNickName() + " " + nick + " " + channelName + "\r\n");
}

