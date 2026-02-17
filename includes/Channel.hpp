/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:21:41 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/06 15:54:43 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <string>
# include <map>
# include <set>

class Server;

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::string _pwd;
		std::map<int , Client*> _clients;
		std::set<int> _operators;
		std::set<int> _invited;
		bool _modeI;
		bool _modeT;
		bool _modeK;
		unsigned long _limit; // 0 = infinite

		public:
			Channel();
			explicit Channel(const std::string &name);
			Channel(const Channel &other);
			Channel &operator=(const Channel &other);
			virtual ~Channel();
			//static void cmdKick(Server &s, Client *c, const Command &cmd);
			void addClient(Client *c);
			void removeClient(int fd);
			void addOperator(int fd);
			const std::map<int , Client*> &getClients() const;
			const std::string &getName() const;
			const std::string &getTopic() const;
			void setTopic(const std::string &topic);
			Client *getClient(const std::string &s);
			bool hasModeT() const;
			bool isOperator(Client* c) const;
			void broadcast(Server &s, const std::string &msg);
			bool hasClient(Client *c);
			void setInviteOnly(bool value);
			void removeKey();
			void setKey(const std::string &key);
			bool hasKey() const;
			void setModeT(bool value);
			void removeOperator(Client *c);
			bool isInviteOnly() const { return _modeI; };
			void addInvite(Client *c);
			void removeInvite(Client *c);
			bool isInvited(Client *c) const;
			const std::string &getKey() const;
			void setLimitUser(int limit);
			void removeLimitUser();
			unsigned long getLimitUser() const;
			bool hasInvite() const;
			std::string getMode();
};

#endif
