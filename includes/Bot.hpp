/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:15:58 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/10 17:51:44 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <string>
#include "Client.hpp"
#include "Server.hpp"

class Bot: public Client
{
	private:
		static std::string S_BOT_0;
		static std::string S_BOT_1;
		static std::string S_BOT_2;
		static std::string S_BOT_3;
		static std::string S_BOT_4;
		static std::string S_BOT_5;
		static std::string S_BOT_6;
		static std::string S_BOT_7;
		int _fdServer;
		Bot();
		Bot(const Client &other);
		Bot &operator=(const Client &other);

	public:
		explicit Bot(int fd);
		virtual ~Bot();
		static Client *createBot(Server &server);
		static std::string responseBot(Server &, Client *, const std::string &);
		static void responseChannel(Server &, Client *, const std::string &);
};

#endif