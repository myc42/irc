/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:17:46 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/09 19:58:03 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// CmdUser.cpp

#include "CommandMgr.hpp"
#include "Parser.hpp"

void CommandMgr::cmdUser(Server &s, Client *c, const Command &cmd)
{

	if (cmd.params.size() < 1)
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_USER) + S_RN);
			return ;
	}
	if ((cmd.params.size() < I_PARAM_USER || !cmd.hasTrailing) && !DEBUG)
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_USER) + S_RN);
			return ;
	}
	if (!Parser::isValidUserName(cmd.params[0]))
	{
		s.clientCast(c->getFd(), Utils::replace(c, S_461, B_COMMAND, S_USER) + S_RN);
		return ;
	}
	if (!c->getUserName().empty())
	{
		s.clientCast(c->getFd(), Utils::replace(c, std::string(S_462), "", "") + S_RN);
		return;
	}
	c->setUserName(cmd.params[0]);
	login(s, c);
}
