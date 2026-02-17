/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macoulib <macoulib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:38:48 by macoulib          #+#    #+#             */
/*   Updated: 2026/02/17 14:20:48 by macoulib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// CmdMode.cpp

#include <cstdlib>
#include <iostream>
#include "CommandMgr.hpp"

static void sendMode(Server &server, Client *client, Channel &channel, const std::string &channelName)
{
	std::string msg = Utils::replace(client, S_324, B_CHANNEL, channelName);
	msg = Utils::replace(client, msg, B_MODE, channel.getMode());
	server.clientCast(client->getFd(), msg + S_RN);
}

static void doCmdMode(Server &server, Client *client, const Command &cmd, const std::string channelName)
{
	std::string targetName;
	if (!server.hasChannel(channelName)) {
		std::string msg = ":ft_irc 403 " + client->getNickName() + " " + channelName + " :No such channel\r\n";
		server.clientCast(client->getFd(), msg);
		return;
	}
	Channel &channel = server.getChannel(channelName);
	if (cmd.params.size() < 2)
		return (sendMode(server, client, channel, channelName));
	std::string flags = cmd.params[1];
	if (flags == "+" || flags == "-")
		return (server.clientCast(client->getFd(), Utils::replace(client, S_501, B_SERVER_NAME, S_SERVER_NAME) + S_RN));
	if (!channel.hasClient(client)) {
		std::string msg = ":ft_irc 442 " + client->getNickName() + " " + channelName + " :You're not on that channel\r\n";
		server.clientCast(client->getFd(), msg);
		return;
	}

	if (!channel.isOperator(client)) {
		std::string msg = ":ft_irc 482 " + client->getNickName() + " " + channelName + " :You're not channel operator\r\n";
		server.clientCast(client->getFd(), msg);
		return;
	}

	char sign = 0;
	size_t paramIndex = 2;

	for (size_t i = 0; i < flags.size(); ++i)
	{
		char c = flags[i];
		if (c == '+' || c == '-') {
			if (sign == 0)
				sign = c;
			else
				return (server.clientCast(client->getFd(), Utils::replace(client, S_501, B_SERVER_NAME, S_SERVER_NAME) + S_RN));
			continue;
		}
		if (sign == 0)
			return (server.clientCast(client->getFd(), Utils::replace(client, S_501, B_SERVER_NAME, S_SERVER_NAME) + S_RN));
		// MODES WITHOUT PARAM
		if (c == 'i')
		{
			if (sign == '+')
				channel.setInviteOnly(true);
			else if (sign == '-')
				channel.setInviteOnly(false);
			else
				return (sendMode(server, client, channel, channelName));
		}
		else if (c == 't')
		{
			if (sign == '+')
				channel.setModeT(true);
			else if (sign == '-')
				channel.setModeT(false);
			else
				return (sendMode(server, client, channel, channelName));
		}
		// MODES WITH PARAM
		else if (c == 'k') {
			if (sign == '+')
			{
				if (paramIndex >= cmd.params.size())
				{
					server.clientCast(client->getFd(), Utils::replace(client, S_461, B_COMMAND, S_MODE) + S_RN);
					continue;
				}
				channel.setKey(cmd.params[paramIndex++]);
			}
			else if (sign == '-')
				channel.removeKey();
			else
				return (sendMode(server, client, channel, channelName));
		}
		else if (c == 'o')
		{
			if (paramIndex >= cmd.params.size())
				return (server.clientCast(client->getFd(), Utils::replace(client, S_461, B_COMMAND, S_MODE) + S_RN));
			Client *target = channel.getClient(cmd.params[paramIndex++]);
			if (!target)
			{
				server.clientCast(client->getFd(), Utils::replace(client, S_401, B_NICKNAME, cmd.params[paramIndex - 1]) + S_RN);
				continue;
			}
			targetName = target->getNickName();
			if (sign == '+')
				channel.addOperator(target->getFd());
			else if (sign == '-')
				channel.removeOperator(target);
			else
				return (sendMode(server, client, channel, channelName));
		}
		else if (c == 'l')
		{
			if (sign == '+')
			{
				if (paramIndex >= cmd.params.size())
					return (server.clientCast(client->getFd(), Utils::replace(client, S_461, B_COMMAND, S_MODE) + S_RN));
				int limit = std::atoi(cmd.params[paramIndex++].c_str());
				channel.setLimitUser(limit);
			}
			else if (sign == '-')
				channel.removeLimitUser();
			else
				return (sendMode(server, client, channel, channelName));
		}
		else
			return (server.clientCast(client->getFd(), Utils::replace(client, S_501, B_SERVER_NAME, S_SERVER_NAME) + S_RN));
	}
	// Broadcast du MODE
	std::string msg;
	if (targetName == "")
		msg = Utils::prefix(client) + " MODE " + channelName + " " + flags + S_RN;
	else
		msg = Utils::prefix(client) + " MODE " + channelName + " " + flags + " " + targetName + S_RN;
	channel.broadcast(server, msg);
}

// Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
// Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
void CommandMgr::cmdMode(Server &server, Client *client, const Command &cmd)
{
	if (!client->getAuthenticated())
	{
		server.clientCast(client->getFd(), std::string(S_451) + S_RN);
		return ;
	}
	if (cmd.params.size() < 1) {
		std::string msg = ":ft_irc 461 " + client->getNickName() + " MODE :Not enough parameters\r\n";
		server.clientCast(client->getFd(), msg);
		return;
	}
	std::string channelName = cmd.params[0];
	if (channelName[0] == '#') // irssi send : mode <user> +i, we ignore it, only channels work
	// 	channelName = "#" + channelName;
		doCmdMode(server, client, cmd, channelName);
}
