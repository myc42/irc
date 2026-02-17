/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 15:26:13 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/05 17:48:06 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Utils.hpp"

Client::Client(int fd):_fd(fd), _in(""), _out(""), _nickName(""), _userName(""), _authenticated(false) { }
Client::~Client() { _fd = Utils::ft_close(_fd);}
int Client::getFd() { return _fd;}
std::string &Client::In() { return _in;}
std::string &Client::Out() { return _out; };
void Client::setNickName(const std::string &nickName) {_nickName = nickName; }
const std::string &Client::getNickName() const { return _nickName; }
void Client::setPwd(const std::string &pwd) {_pwd = pwd; }
const std::string &Client::getPwd() const { return _pwd; }
void Client::setUserName(const std::string &userName) {_userName = userName; }
const std::string &Client::getUserName() const { return _userName; }
void Client::setAuthenticated(bool authenticated) {_authenticated = authenticated; }
bool Client::getAuthenticated() const { return _authenticated; }
const std::string &Client::getHost() const { return _host; }
void Client::setHost(const std::string &host) { _host = host; }
void Client::setFd(int fd) { _fd = fd;}
