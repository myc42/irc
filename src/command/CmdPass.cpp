/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:16:26 by ybouroga          #+#    #+#             */
/*   Updated: 2026/01/29 18:18:24 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// CmdPass.cpp

#include "CommandMgr.hpp"

void CommandMgr::cmdPass(Server &s, Client *c, const Command &cmd)
{
	(void)s;
	if (!cmd.params.empty() && !c->getAuthenticated())
		c->setPwd(cmd.params[0]);
	login(s, c);
}
