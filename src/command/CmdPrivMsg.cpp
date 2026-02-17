/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macoulib <macoulib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 20:30:05 by knehal            #+#    #+#             */
/*   Updated: 2026/02/17 13:57:21 by macoulib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMgr.hpp"
#include "CommandMgr.hpp"
#include "Parser.hpp"
#include "Bot.hpp"

static void bot(Server &s, Client *c, const std::string &target, const std::string &text)
{
	if (target == S_BOT_NICK)
	{
		Client *dest = s.getClient(target);
		if (dest)
			s.clientCast(c->getFd(), Bot::responseBot(s, c, text));
	}
	else if(Parser::toUpper(target) == Parser::toUpper(S_CHANNEL_BOT))
	{
		if (!s.hasChannel(target))
		 	return ;
		Channel &ch = s.getChannel(target);
		if (!ch.hasClient(c))
			return ;
		Bot::responseChannel(s, c, text);
	}
}

void CommandMgr::cmdPrivMsg(Server &s, Client *c, const Command &cmd)
{
    if (!c->getAuthenticated())
    {
        s.clientCast(c->getFd(), Utils::replace(c, std::string(S_451), "", "") + S_RN);
        return;
    }
    if (cmd.params.empty())
    {
        s.clientCast(c->getFd(), ":ft_irc 411 :No recipient given (PRIVMSG)\r\n");
        return;
    }
    if (!cmd.hasTrailing || cmd.trailing.empty())
    {
        s.clientCast(c->getFd(), ":ft_irc 412 :No text to send\r\n");
        return;
    }

    std::vector<std::string> targets = Parser::parseJoin(cmd.params[0]);
    std::string text = cmd.trailing;

    std::string prefix = ":" + c->getNickName() + "!" + c->getUserName() + "@" + c->getHost();

    for (size_t i = 0; i < targets.size(); i++)
    {
        std::string target = targets[i];
        if (target.empty()) continue;

		std::string msg = prefix + " PRIVMSG " + target + " :" + text + S_RN;
		if (s.getExtra())
		{
			if (text.size() > 1 && text[0] == '\x01')
			{
				if (text.find("DCC SEND") != std::string::npos)
				{
					cmdDccSend(s, c, target, text);
					continue;
				}
			}
        }
        if (target[0] == '#' || target[0] == '&')
        {
            if (!s.hasChannel(target))
            {
                s.clientCast(c->getFd(), Utils::replace(c, S_403, B_S1, target) + S_RN);
                continue;
            }

            Channel &ch = s.getChannel(target);
            if (!ch.hasClient(c))
            {
                s.clientCast(c->getFd(), ":ft_irc 404 " + target + " :Cannot send to channel\r\n");
                continue;
            }

            std::map<int, Client*> m = ch.getClients();
            for (std::map<int, Client*>::iterator it = m.begin(); it != m.end(); ++it)
            {
                if (it->second && it->second != c)
                    s.clientCast(it->first, msg);
            }
        }
        else
        {
            Client *dest = s.getClient(target);
            if (!dest)
            {
                s.clientCast(c->getFd(), ":ft_irc 401 " + target + " :No such nick/channel\r\n");
                continue;
            }
            s.clientCast(dest->getFd(), msg);
        }
		bot(s, c, target, text);
    }
}

