/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMgr.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:29:04 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/11 17:21:30 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// CommandMgr.cpp
#include <iostream>
#include "CommandMgr.hpp"
#include "Parser.hpp"
#include "Command.hpp"
#include "Utils.hpp"

CommandMgr::~CommandMgr() {}

void CommandMgr::cmd(Server &s, Client *c, const std::string &str)
{
	Command cmd = Parser::parseLine(str);
	Utils::printVar("str", str);
	Utils::printVar("cmd.name", cmd.name);
	Utils::printVar("cmd.params", cmd.params);
	Utils::printVar("cmd.trailing", cmd.trailing);
	std::cout << std:: endl;

	if (cmd.name == S_CAP)
		return;
	else if (cmd.name == S_KICK)
		cmdKick(s, c, cmd);
	else if (cmd.name == S_INVITE)
		cmdInvite(s, c, cmd);
	else if (cmd.name == S_JOIN)
		cmdJoin(s, c, cmd);
	else if (cmd.name == S_QUIT)
		cmdQuit(s, c, cmd);
	else if (cmd.name == S_MODE)
		cmdMode(s, c, cmd);
	else if (cmd.name == S_NICK)
		cmdNick(s, c, cmd);
	else if (cmd.name == S_PART)
		cmdPart(s, c, cmd);
	else if (cmd.name == S_PASS)
		cmdPass(s, c, cmd);
	else if (cmd.name == S_PING)
		cmdPing(s, c , cmd);
	else if (cmd.name == S_PRIVMSG)
		cmdPrivMsg(s, c, cmd);
	else if (cmd.name == S_PRIVMSG || (cmd.name == S_MSG && DEBUG))
		cmdPrivMsg(s, c, cmd);
	else if (cmd.name == S_TOPIC)
		cmdTopic(s, c, cmd);
	else if (cmd.name == S_USER)
		cmdUser(s, c, cmd);
	else if (cmd.name == S_WHO)
		cmdWho(s, c, cmd);
	else if (cmd.name == S_WHOIS)
		return;
	else if (cmd.name == "")
		return;
	// else if (cmd.name == "Z" || (cmd.name == S_MSG && DEBUG)) // TODO to delete
	// {
	// 	s.clientCast(c->getFd(), std::string("USER=[") + c->getUserName() + ']' + S_RN);
	// 	s.clientCast(c->getFd(), std::string("NICK=[") + c->getNickName() + ']' + S_RN);
	// 	s.printChannel(c->getFd(), s);
	// }
	else
		s.clientCast(c->getFd(), Utils::replace(c, std::string(S_421), B_COMMAND, cmd.name) + S_RN);
}

void CommandMgr::login(Server &s, Client *c)
{
	if (!c->getNickName().empty() && !c->getUserName().empty() && !c->getAuthenticated())
	{
		if (c->getPwd() != s.getPwd())
		{
			s.clientCast(c->getFd(), Utils::replace(c, std::string(S_464), "", "") + S_RN);
			return ;
		}
		c->setAuthenticated(true);
		Utils::welcome(s, c);
	}
}
