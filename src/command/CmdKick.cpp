/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 18:50:50 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/11 21:06:20 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMgr.hpp"

void CommandMgr::cmdKick(Server &s, Client *c, const Command &cmd)
{
	if (!c->getAuthenticated())
	{
		s.clientCast(c->getFd(), Utils::replace(c, std::string(S_451), "", "") + S_RN);
		return;
	}
	if (cmd.params.size() < 2)
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_KICK) + S_RN);
		return ;
	}
	std::string channelName = cmd.params[0];
	if (channelName[0] != '#')
		channelName = "#" + channelName;
	std::string kickedName = cmd.params[1];
	std::string reason = cmd.hasTrailing ? cmd.trailing : " ";

	if (!s.hasChannel(channelName))
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_403, B_S1, channelName) + S_RN);
		return ;
	}
	Channel &channel = s.getChannel(channelName);

	//  Check if client in the channel
	if (!channel.hasClient(c)) {
		std::string msg = ":ft_irc 442 " + c->getNickName() + " " + channelName + " :You're not on that channel\r\n";
		s.clientCast(c->getFd(), msg);
		return;
	}
	// Check is the kick is on the channel
	if (!channel.getClient(kickedName))
	{
		std::string str = Utils::replace(c, S_441, B_CHANNEL, cmd.params[0]);
		// str = Utils::replace(c, str, B_NICK, c->getNickName());
		str = Utils::replace(c, str, B_TARGET, kickedName);
		s.clientCast(c->getFd(), str + S_RN);
		return ;
	}
	Client* kicked = channel.getClient(kickedName);
	// Check is the client is an operateur
	if (!channel.isOperator(c)) {
		std::string msg = ":ft_irc 482 " + c->getNickName() + " " + channelName + " :You're not channel operator\r\n";
		s.clientCast(c->getFd(), msg);
		return;
	}
	std::string str;
	if (cmd.hasTrailing)
		str = Utils::replace(c, S_MESSAGE_KICK_REASON, B_KICKED, kickedName);
	else
		str = Utils::replace(c, S_MESSAGE_KICK, B_KICKED, kickedName);
	str = Utils::replace(c, str, B_USER, c->getUserName());
	str = Utils::replace(c, str, B_CHANNEL, channelName);
	if (cmd.hasTrailing)
		str = Utils::replace(c, str, B_REASON, reason);
	s.clientsCast(channel.getClients(), str + S_RN);
	channel.removeClient(kicked->getFd());
	if (channel.getClients().empty())
		s.deleteChannel(channelName);
}
