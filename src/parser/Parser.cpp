/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knehal <knehal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:47:12 by knehal            #+#    #+#             */
/*   Updated: 2026/02/17 13:32:36 by knehal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

void Parser::skipSpaces(const std::string &s, size_t &i)
{
	while (i < s.size() && s[i] == ' ')
		i++;
}

std::string Parser::readToken(const std::string &s, size_t &i)
{
	size_t start = i;
	while (i < s.size() && s[i] != ' ')
		i++;
	return s.substr(start, i - start);
}

std::string Parser::toUpper(const std::string &s)
{
	std::string out = s;
	for (size_t i = 0; i < out.size(); i++)
		out[i] = std::toupper(static_cast<unsigned char>(out[i]));
	return out;
}

Command Parser::parseLine(std::string line)
{
	Command cmd;

	if (line.size() >= 2 && line[line.size() - 2] == '\r' && line[line.size() - 1] == '\n')
	line.erase(line.size() - 2);

	size_t i = 0;
	skipSpaces(line, i);

	if (i < line.size() && line[i] == ':')
	{
		i++;
		cmd.prefix = readToken(line, i);
		skipSpaces(line, i);
	}

	if (i >= line.size())
		return cmd;
	cmd.name = toUpper(readToken(line, i));

	while (i < line.size())
	{
		skipSpaces(line, i);
		if (i >= line.size())
			break;
		if (line[i] == ':')
		{
			i++;
			cmd.trailing = line.substr(i);
			cmd.hasTrailing = true;
			break;
		}
		cmd.params.push_back(readToken(line, i));
	}
	return cmd;
}

std::vector<std::string> Parser::parseJoin(std::string s)
{
	size_t cursor;
	size_t pos;
	std::vector<std::string> ret;

	cursor = 0;
	pos = 0;
	while ((pos = s.find(',', cursor)) != std::string::npos)
	{
		ret.push_back(s.substr(cursor, pos - cursor));
		cursor = pos + 1;
	}
	ret.push_back(s.substr(cursor));
	return ret;
}

bool Parser::isIrcSpecial(unsigned char c)
{
    return (c == '[' || c == ']' || c == '\\' || c == '`' ||
            c == '^' || c == '_' || c == '{'  || c == '|' || c == '}');
}

bool Parser::isForbiddenNickChar(unsigned char c)
{
    if (c == ' ' || c == ',' || c == ':') return true;
    if (c == '#' || c == '&') return true;
    if (c < 32 || c == 127) return true;
    return false;
}

bool Parser::isValidNickIRC(const std::string &nick)
{
    if (nick.empty()) return false;
    if (nick.size() > 9) return false;

    unsigned char c0 = (unsigned char)nick[0];
    if (isForbiddenNickChar(c0)) return false;
    if (!(std::isalpha(c0) || isIrcSpecial(c0))) return false;

    for (size_t i = 1; i < nick.size(); ++i)
    {
        unsigned char c = (unsigned char)nick[i];
        if (isForbiddenNickChar(c)) return false;
        if (!(std::isalnum(c) || isIrcSpecial(c) || c == '-')) return false;
    }
    return true;
}

bool Parser::isValidUserName(const std::string &user)
{
    if (user.empty())
        return false;

    for (size_t i = 0; i < user.size(); ++i)
    {
        unsigned char c = (unsigned char)user[i];

        if (c == ' ' || c == '\t')
            return false;
        if (c == '\r' || c == '\n')
            return false;
        if (c < 32 || c == 127)
            return false;
    }
    return true;
}
