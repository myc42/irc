/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalMgr.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 15:48:49 by ybouroga          #+#    #+#             */
/*   Updated: 2026/01/21 17:16:44 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// SignalMgr.cpp
#include "SignalMgr.hpp"
#include "constants.h"
#include <csignal>
#include <iostream>
#include <cstdlib>

volatile sig_atomic_t SignalMgr::_signal = 0;

void SignalMgr::setupSignal()
{
	std::signal(SIGINT, setSignal);
	std::signal(SIGQUIT, setSignal);
	std::signal(SIGPIPE, SIG_IGN);
}

void SignalMgr::setSignal(int sig)
{
	_signal = sig;
}

int SignalMgr::handleSignal()
{
	if (_signal == SIGINT)
	{
		std::cout << S_SIGINT << std::endl;
	}
	else if (_signal == SIGPIPE)
		std::cout << S_SIGPIPE << std::endl;
	else if (_signal == SIGQUIT)
	{
		std::cout << S_SIGQUIT << std::endl;
	}
	return _signal;
}
