/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:16:59 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/16 22:17:13 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMgr.hpp"
#include "Utils.hpp"

void CommandMgr::cmdPing(Server &s, Client *c, const Command &cmd)
{
	if (cmd.hasTrailing)
		s.clientCast(c->getFd(), std::string(S_PONG) + " " + S_SERVER_NAME + " :" + cmd.trailing  + S_RN);
	else if (!cmd.params.empty())
		s.clientCast(c->getFd(), std::string(S_PONG) + " " + S_SERVER_NAME + " " + cmd.params[0]  + S_RN);
	// Utils::printVar("c->getFd()", c->getFd());
	// Utils::printVar("cmd.hasTrailing", cmd.hasTrailing);
	// Utils::printVar("cmd.params", cmd.params);
}
