/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 15:08:02 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/10 18:22:09 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <map>
# include <vector>
# include <poll.h>
# include "Channel.hpp" 

class Bot;

class Server
{
	private:
		int _fd;
		std::string	_port;
		std::string	_pwd;
		bool		_isRunning;
		std::map<int, Client*> _clients;
		std::map<std::string, Channel> _channels;
		std::vector<struct pollfd> _pollFds;
		std::string _startTime;
		//Bot *_bot;
		bool _extra;
		Server();
		Server(const Server &other);
		Server	&operator=(const Server &other);
		Client *getClient(int fd);
		void acceptNewClient();
		void readClient(size_t index);
		void writeClient(size_t index);
		void clientDisconnect(size_t index, const std::string &s);
		void initSocket();

	public:
		Server(std::string port, const std::string pwd, bool extra);
		virtual ~Server();
		void clean();
		void run();
		void setIsRunning(bool b);
		void broadcast(int fdBroadcast, const std::string &s);
		void clientCast(int fd, const std::string &s);
		void clientsCast(const std::map<int, Client *> &clients, const std::string &s);
		bool hasChannel(const std::string &s);
		Channel &getChannel(const std::string &s);
		void setPwd(const std::string &s);
		const std::string &getPwd() const;
		Channel &addChannel(const std::string &s);
		void printChannel(int, Server &) const;
		const std::string &getStartTime() const;
		Client *getClient(const std::string &);
		const std::map<int, Client*> &getClients() const;
		const std::map<std::string, Channel> &getChannels() const;
		void deleteChannel(const std::string &s);
		bool getExtra();
		void addClient(Client *);
		void addPoll(int fd, short events);
		void setPollOut(int fd);
		void clientQuit(int fd);
};

#endif