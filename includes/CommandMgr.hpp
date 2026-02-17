/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMgr.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:02:07 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/16 15:54:18 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include "Client.hpp"
# include "Server.hpp"
# include "Command.hpp"
# include "constants.h"
# include "Utils.hpp"
# include <string>
# include <iostream>

class CommandMgr
{
	private:
		CommandMgr();
		CommandMgr(const Server &other);
		CommandMgr	&operator=(const Server &other);
		static void cmdKick(Server &s, Client *c, const Command &cmd);
		static void cmdNick(Server &s, Client *c, const Command &cmd);
		static void cmdPart(Server &s, Client *c, const Command &cmd);
		static void cmdPass(Server &s, Client *c, const Command &cmd);
		static void cmdUser(Server &s, Client *c, const Command &cmd);
		static void cmdJoin(Server &s, Client *c, const Command &cmd);
		static void cmdTopic(Server &s, Client *c, const Command &cmd);
		static void cmdPing(Server &s, Client *c, const Command &cmd);
		static void cmdMode(Server &s, Client *c, const Command &cmd);
		static void cmdPrivMsg(Server &s, Client *c, const Command &cmd);
		static void cmdWho(Server &s, Client *c, const Command &cmd);
		static void cmdInvite(Server &s, Client *c, const Command &cmd);
		static void cmdQuit(Server &s, Client *c, const Command &cmd);
		static void cmdDccSend(Server &server, Client *sender, const std::string &targetNick, const std::string &dccMessage);

	public:
		virtual ~CommandMgr();
		static void cmd(Server &server, Client *c, const std::string &s);
		static void login(Server &s, Client *c);

};

#endif