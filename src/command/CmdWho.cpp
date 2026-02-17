/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdWho.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knehal <knehal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 20:58:29 by knehal            #+#    #+#             */
/*   Updated: 2026/02/17 13:32:10 by knehal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMgr.hpp"

void CommandMgr::cmdWho(Server &s, Client *c, const Command &cmd)
{
    if (!c->getAuthenticated()) {
        s.clientCast(c->getFd(), Utils::replace(c, std::string(S_451), "", "") + S_RN);
        return;
    }

    std::string mask = cmd.params.empty() ? "*" : cmd.params[0];
    std::string me = c->getNickName();

    bool onlyOps = (cmd.params.size() >= 2 && (cmd.params[1] == "o" || cmd.params[1] == "O"));

    std::string targetChannel = mask;
    bool isChannelSearch = false;

    if (!mask.empty() && (mask[0] == '#' || mask[0] == '&'))
    {
        isChannelSearch = true;
    }
    else if (mask != "*" && s.hasChannel("#" + mask))
    {
        targetChannel = "#" + mask;
        isChannelSearch = true;
    }

    if (isChannelSearch && s.hasChannel(targetChannel))
    {
        Channel &ch = s.getChannel(targetChannel);
        const std::map<int, Client*> &m = ch.getClients();

        for (std::map<int, Client*>::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            Client *u = it->second;
            if (!u) continue;

            bool isOp = ch.isOperator(u);

            if (onlyOps && !isOp) continue;

            std::string status = "H";
            if (isOp) status += "@";

            std::string line = ":ft_irc 352 " + me + " " + targetChannel + " " +
                u->getUserName() + " " + u->getHost() + " ft_irc " +
                u->getNickName() + " " + status + " :0 " + u->getNickName() + S_RN;
            s.clientCast(c->getFd(), line); //
        }
    }
    else if (mask == "*")
    {
        const std::map<int, Client*> &allClients = s.getClients();
        for (std::map<int, Client*>::const_iterator it = allClients.begin(); it != allClients.end(); ++it)
        {
            Client *u = it->second;
            if (!u || !u->getAuthenticated()) continue;
            s.clientCast(c->getFd(), ":ft_irc 352 " + me + " * " + u->getUserName() + " " + u->getHost() + " ft_irc " + u->getNickName() + " H :0 " + u->getNickName() + S_RN);
        }
    }
    s.clientCast(c->getFd(), ":ft_irc 315 " + me + " " + mask + " :End of WHO list\r\n");
}
