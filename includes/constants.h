/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 21:34:04 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/16 22:15:49 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# ifndef DEBUG
# define DEBUG 0
# endif

# define MAX_BUFFER 4096
# define M1 -1
# define S_001 ":<servername> 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host>"
# define S_002 ":<servername> 002 <nick> :Your host is <servername>, running version <version>"
# define S_003 ":<servername> 003 <nick> :This server was created <date>"
# define S_004 ":<servername> 004 <nick> <servername> <version> <available user modes> <available channel modes>"
# define S_324 ":<servername> 324 <nick> <channel> <mode>"
# define S_331 ":<servername> 331 <nick> <channel> :No topic is set"
# define S_332 ":<servername> 332 <nick> <channel> :<topic>"
# define S_353 ":<servername> 353 <nick> = <channel> :<names>"
# define S_366 ":<servername> 366 <nick> <channel> :End of NAMES list"
# define S_401 ":<servername> 401 <nick> <nickname> :No such nick/channel"
# define S_403 ":<servername> 403 <nick> <s1> :No such channel"
# define S_421 ":<servername> 421 <nick> <command> :Unknown command"
# define S_431 ":<servername> 431 <nick> :No nickname given"
# define S_432 ":<servername> 432 <nick> :Erroneous nickname"
# define S_433 ":<servername> 443 <nick> :Nickname is already in use"
# define S_441 ":<servername> 441 <nick> <target> <channel> :They aren't on that channel"
# define S_442 ":<servername> 442 <nick> <channel> :You're not on that channel"
# define S_451 ":<servername> 451 <nick> :You have not registered"
# define S_461 ":<servername> 461 <nick> <command> :Not enough parameters"
# define S_462 ":<servername> 462 <nick> :Unauthorized command (already registered)"
# define S_464 ":<servername> 464 <nick> :Password incorrect"
# define S_475 ":<servername> 475 <nick> <channel> :Cannot join channel (+k)"
# define S_482 ":<servername> 482 <nick> <channel> :You're not channel operator"
# define S_501 ":<servername> 501 <nick> :Unknown MODE flag"
# define S_ERROR_FCNTL "Error: fcntl set non blocking"
# define S_ERROR_PARAMS "Error: Run with 2 paramaters ./ircserv <port><password>"
# define S_GETADDRINFO "Error: getaddrinfo"
# define S_ERROR_PORT "Error: Port range is [1, 65535]"
# define S_ERROR_SETSOCKOPT "Error: setsockopt failed"
# define S_ERROR_SOCKET "Error: init socket failed"
# define S_ERROR_LISTEN "Error: listen failed"
# define S_CLIENT_CONNECTED "Client: Try connecting to the server ...\r\n"
# define S_CLIENT_FD "Client fd = "
# define S_DISCONNECT "Client disconnect fd = "
# define S_MESSAGE " messge = "
# define S_BUFFER_OVERFLOW "Buffer overflow \r\n"
# define S_CLIENT_CLOSE "Client close"
# define S_CLIENT_CLOSE_RECV "Client close: RECV"
# define S_CLIENT_CLOSE_SEND "Client close: SEND"
# define S_CLIENT_MESSAGE "Client message = "
# define S_CLIENT_MESSAGE_1 ":nick!user@host PRIVMSG target :"
# define S_RN "\r\n"
# define C_R '\r'
# define C_N '\n'
# define S_SIGINT "SIGINT sent"
# define S_SIGPIPE "SIGPIPE sent but ignored"
# define S_SIGQUIT "SIGQUIT sent"
# define S_ERROR_POLL "Error: Poll"
# define S_SERVER_NAME "ft_irc"
# define S_CAP "CAP"
# define S_KICK "KICK"
# define S_INVITE "INVITE"
# define S_JOIN "JOIN"
# define S_QUIT "QUIT"
# define S_MODE "MODE"
# define S_NICK "NICK"
# define S_PART "PART"
# define S_PASS "PASS"
# define S_PING "PING"
# define S_PONG "PONG"
# define S_PRIVMSG "PRIVMSG"
# define S_MSG "MSG"
# define S_TOPIC "TOPIC"
# define S_USER "USER"
# define S_WHO "WHO"
# define S_WHOIS  "WHOIS"
# define B_COMMAND "<command>"
# define B_S1 "<s1>"
# define S_ERR_CHANNEL "Channel name not exists"
# define S_ERROR_PWD "Error: Password must not be empty"
# define S_ERR_CHANNEL_EXISTS "Channel name exists"
# define S_MESSAGE_JOIN ":<nick>!<user>@host JOIN <channel>"
# define B_NICK "<nick>"
# define B_USER "<user>"
# define B_CHANNEL "<channel>"
# define B_TOPIC "<topic>"
# define S_MESSAGE_KICK_REASON ":<nick>!<user>@host KICK <channel> <kicked> :<reason>"
# define S_MESSAGE_KICK ":<nick>!<user>@host KICK <channel> <kicked>"
# define B_KICKED "<kicked>"
# define B_REASON "<reason>"
# define B_SERVER_NAME "<servername>"
# define B_VERSION "<version>"
# define S_VERSION "1.0"
# define B_DATE "<date>"
# define S_UNKNOWN "unknown"
# define B_HOST "<host>"
# define B_USER_MODE "<available user modes>"
# define S_USER_MODE ""
# define S_CHANNEL_MODE ""
# define S_USER_MODE_2 "o" // operator
# define S_CHANNEL_MODE_2 "biltn" // ban, invite only, limit, topic protection, no external message
# define B_CHANNEL_MODE "<available channel modes>"
# define S_ERROR_FTIRC "Error ft_irc: "
# define I_PARAM_USER 3 // or 3 for be compliant to RFC
# define S_HOST_S1 "@host <s1>"
# define C_OPERATOR '@'
# define B_MODE "<mode>"
# define S_ERROR_BOT "Error: Bot cannot be create"
# define S_IAM_BOT ": I'am a bot"
# define S_BOT_CHANNEL "bot_channel"
# define S_BOT_USER "ro"
# define S_BOT_NICK "bot$"
# define B_NAMES "<names>"
# define B_NICKNAME "<nickname>"
# define B_TARGET "<target>"
# define S_CHANNEL_BOT "#b"
# define S_BOT_TOPIC ":Ici on ne parle pas !!!"


#endif
