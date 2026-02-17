/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:30:58 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/10 18:26:35 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://www.rfc-editor.org/rfc/rfc1459
// https://www.rfc-editor.org/rfc/rfc2812.html

#include <iostream>
#include <cstdlib>
#include "constants.h"
#include "Server.hpp"
#include "Utils.hpp"

void ft_irc(std::string port, std::string pwd)
{
	try
	{
		bool extra = 0;
		std::srand(0);
		std::cout << S_SERVER_NAME << ":" << port << ":" << pwd << std::endl;
		if (! Utils::isAllDigits(port) || port.size() > 5 || std::atoi(port.c_str()) <= 0  || std::atoi(port.c_str()) > 65535)
			throw std::runtime_error(S_ERROR_PORT);
		if (pwd.empty())
			throw std::runtime_error(S_ERROR_PWD);
		Server(port, pwd, extra).run();
	}
	catch (std::exception &ex)
	{
		std::cerr << S_ERROR_FTIRC << ex.what() << std::endl;
	}
}

int	main(int argc, char *argv[])
{
	if (argc == 3)
		ft_irc(argv[1], argv[2]);
	else std::cout << S_ERROR_PARAMS << std::endl;
}
