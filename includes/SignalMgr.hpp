/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalMgr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 15:45:58 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/13 17:19:18 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_MGR_HPP
# define SIGNAL_MGR_HPP

#include "Server.hpp"
#include <csignal>

class SignalMgr
{
	private:
		static volatile sig_atomic_t _signal;
		SignalMgr();
		static void setSignal(int sig);

	public:
		static void setupSignal();
		static int handleSignal();
};


#endif
