/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:26:53 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/10 17:51:25 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "constants.h"
#include "Utils.hpp"
#include "CommandMgr.hpp"
#include <iostream>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

std::string Bot::S_BOT_0 = "Not Now";
std::string Bot::S_BOT_1 = "I'm busy";
std::string Bot::S_BOT_2 = "I'm leaving";
std::string Bot::S_BOT_3 = "Maybe later";
std::string Bot::S_BOT_4 = "Join my channel #b with this pass: 0";
std::string Bot::S_BOT_5 = "I'am not sure to understand";
std::string Bot::S_BOT_6 = "Yes";
std::string Bot::S_BOT_7 = "No";


Bot::Bot(int fd): Client(fd), _fdServer(-1) {}

static void setFcntl(int n, int fds[2] )
{
	if (fcntl(fds[n], F_SETFL, O_NONBLOCK) < 0)
	{
		fds[0] = Utils::ft_close(fds[0]);
		fds[1] = Utils::ft_close(fds[1]);
		throw std::runtime_error(S_ERROR_FCNTL);
	}
}

Client *Bot::createBot(Server & server)
{
	int fds[2];
	std::cout << "Create bot" << std::endl;
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0)
	{
		std::cout << S_ERROR_BOT << std::endl;
		return NULL;
	}
	setFcntl(0, fds);
	setFcntl(1, fds);
	Bot *bot = new Bot(fds[0]);
	if (bot == NULL)
	{
		Utils::ft_close(fds[0]);
		Utils::ft_close(fds[1]);
		std::cout << S_ERROR_BOT << std::endl;
		return NULL;
	}

	bot->_fdServer = fds[1];
	bot->setUserName(S_BOT_USER);
	bot->setNickName(S_BOT_NICK);
	bot->setPwd(server.getPwd());
	bot->setAuthenticated(true);
	CommandMgr::cmd(server, bot, std::string(S_JOIN) + " " + S_CHANNEL_BOT);
	CommandMgr::cmd(server, bot, std::string(S_TOPIC) + " " + S_CHANNEL_BOT + " " + S_BOT_TOPIC);
	CommandMgr::cmd(server, bot, std::string(S_MODE) + " " + S_CHANNEL_BOT + " +k 0");
	server.addClient(bot);
	server.addPoll(bot->getFd(), POLLIN | POLLOUT);
	return bot;
}
std::string Bot::responseBot(Server& server, Client *c, const std::string &s)
{
	(void) server;
	std::string ret;
	std::string prefix = ":" + std::string(S_BOT_NICK) + "!" + S_BOT_USER + "@" + c->getHost();
	ret = prefix + " " + S_PRIVMSG + " " + c->getNickName() + " ";
	ret += "Hey " + c->getNickName() + "! ";
	int ran = std::rand() % 4;
	const std::string *responses1[] = { &S_BOT_0, &S_BOT_1, &S_BOT_2, &S_BOT_3 };
	if (s.find('?') == std::string::npos)
		ret += *responses1[ran];
	else
	{
		const std::string *responses2[] = { &S_BOT_4, &S_BOT_5, &S_BOT_6, &S_BOT_7 };
		ret += *responses2[ran];
	}
	ret += S_RN;
	return ret;
}

void Bot::responseChannel(Server& server, Client *c, const std::string &s)
{
	if (std::rand() % 4 == 0)
		CommandMgr::cmd(server, server.getClient(S_BOT_NICK), std::string(S_KICK) + " " + S_CHANNEL_BOT + " " + c->getNickName() + " " + S_BOT_TOPIC + " - " + s);
}

Bot::~Bot()
{
	_fdServer = Utils::ft_close(_fdServer);
	setFd(Utils::ft_close(getFd()));
}

