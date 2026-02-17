/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:15:56 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/09 17:37:56 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMgr.hpp"

void CommandMgr::cmdPart(Server &s, Client *c, const Command &cmd)
{
	if (!c->getAuthenticated())
	{
		s.clientCast(c->getFd(), Utils::replace(c, std::string(S_451), "", "") + S_RN);
		return ;
	}
	if (cmd.params.size() < 1)
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_PART) + S_RN);
		return ;
	}
	std::string channelName = cmd.params[0];
	if (channelName[0] != '#')
		channelName = std::string("#") +  channelName;
	if (!s.hasChannel(channelName))
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_403, B_S1, channelName) + S_RN);
		return;
	}
	Channel &channel = s.getChannel(channelName);
	if (!channel.hasClient(c))
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_442, B_CHANNEL, channelName) + S_RN);
		return ;
	}
	std::string str = ":" + c ->getNickName() + "!" + c->getUserName() + Utils::replace(c, S_HOST_S1, B_S1, S_PART) + " " + channelName + S_RN;
	s.clientsCast(channel.getClients(), str);
	channel.removeClient(c->getFd());
	if (channel.getClients().empty())
		s.deleteChannel(channelName);
}
