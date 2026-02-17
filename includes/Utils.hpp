/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:21:07 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/06 18:53:08 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include "Server.hpp"
# include "Client.hpp"

class Utils
{
	private:
		Utils();

		public:
			static bool isAllDigits(const std::string &s);
			static void removeCR(std::string &s);
			static bool isValidNickname(const std::string& nick);
			static void welcome(Server &s, Client *c);
			static std::string getDate();
			static void printVar(const std::string &s, int n);
			static void printVar(const std::string &s1, const std::string &s2);
			static void printVar(const std::string &s1, const std::vector<std::string> &tab);
			static int ft_close(int fd);
			static std::string prefix(Client *c);
			static std::string replace(std::string s, const std::string &sOld, const std::string &sNew);
			static std::string replace(Client *c, std::string s, const std::string &sOld, const std::string &sNew);

};

#endif

