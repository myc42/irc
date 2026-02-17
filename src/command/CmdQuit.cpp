/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdQuit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knehal <knehal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 19:08:18 by knehal            #+#    #+#             */
/*   Updated: 2026/02/17 13:33:40 by knehal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMgr.hpp"

void CommandMgr::cmdQuit(Server &s, Client *c, const Command &cmd)
{
    std::string reason = "Client Quit";
    if (cmd.hasTrailing)
        reason = cmd.trailing;

    std::string quitMsg =
        ":" + c->getNickName() + "!" +
        c->getUserName() + "@localhost QUIT :" +
        reason + S_RN;

    // s.broadcast(c->getFd(), quitMsg);
    // s.clientCast(c->getFd(), quitMsg);
    s.broadcast(M1, quitMsg);
    s.clientQuit(c->getFd());
}
