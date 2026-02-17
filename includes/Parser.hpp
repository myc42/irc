/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:03:39 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/13 19:49:00 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include "Command.hpp"

class Parser
{
	private:
		Parser();
		static void skipSpaces(const std::string &s, size_t &i);
		static std::string readToken(const std::string &s, size_t &i);
		static bool isIrcSpecial(unsigned char c);
		static bool isForbiddenNickChar(unsigned char c);

	public:
		static std::string toUpper(const std::string &s);
		static Command parseLine(std::string s);
		static std::vector<std::string> parseJoin(std::string s);
		static bool isValidNickIRC(const std::string &nick);
		static bool isValidUserName(const std::string &user);
};

#endif
