/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:19:08 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/05 17:48:15 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

class Client
{
	private:
		int _fd;
		std::string _in;
		std::string _out;
		std::string _nickName;
		std::string _pwd;
		std::string _userName;
		std::string _host;
		bool _authenticated;
		Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

	public:
		explicit Client(int fd);
		virtual ~Client();
		int getFd();
		std::string &In();
		std::string &Out();
		void setNickName(const std::string &s);
		const std::string &getNickName() const;
		void setPwd(const std::string &s);
		const std::string &getPwd() const;
		void setUserName(const std::string &s);
		const std::string &getUserName() const;
		void setAuthenticated(bool authenticated);
		bool getAuthenticated() const;
		const std::string &getHost() const;
		void setHost(const std::string &);
		void setFd(int fd);

};

#endif
