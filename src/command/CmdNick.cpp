/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:22:18 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/06 21:02:08 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// cmdNick.cpp

#include "CommandMgr.hpp"
#include "Parser.hpp"

void CommandMgr::cmdNick(Server &s, Client *c, const Command &cmd)
{
	if (cmd.params.empty())
	{
		s.clientCast(c->getFd(), Utils::replace(NULL, S_431, B_NICK, "*") + S_RN);
		return;

	}
	if (!Parser::isValidNickIRC(cmd.params[0]))
	{
		s.clientCast(c->getFd(), Utils::replace(NULL, S_432, B_NICK, cmd.params[0]) + S_RN);
		return;
	}
	if (c->getNickName() != cmd.params[0] && s.getClient(cmd.params[0]))
	{
		s.clientCast(c->getFd(), Utils::replace(NULL, S_433, B_NICK, cmd.params[0]) + S_RN);
		return ;
	}
	std::string oldName = c->getNickName();
	c->setNickName(cmd.params[0]);
	if (c->getAuthenticated() && oldName != cmd.params[0])
		s.clientsCast(s.getClients(), Utils::prefix(c) + " " + S_NICK + " :" + cmd.params[0] + S_RN);
	login(s, c);
}
