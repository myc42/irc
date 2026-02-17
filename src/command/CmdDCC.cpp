/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdDCC.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macoulib <macoulib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:50:43 by macoulib          #+#    #+#             */
/*   Updated: 2026/02/17 14:20:42 by macoulib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMgr.hpp"

void CommandMgr::cmdDccSend(Server &server, Client *sender, const std::string &targetNick, const std::string &dccMessage)
{
    Client *target = server.getClient(targetNick);
    if (!target)
    {
        server.clientCast(sender->getFd(),
            ":ft_irc 401 " + targetNick + " :No such nick/channel\r\n");
        return;
    }

    std::string msg = ":" + sender->getNickName() + "!" +
                      sender->getUserName() + "@" +
                      sender->getHost() +
                      " PRIVMSG " + targetNick +
                      " :" + dccMessage + "\r\n";

    server.clientCast(target->getFd(), msg);
}

