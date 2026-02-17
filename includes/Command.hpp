/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:54:04 by ybouroga          #+#    #+#             */
/*   Updated: 2026/02/13 19:48:34 by ybouroga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Command.hpp
#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>

struct Command
{
	std::string prefix;
	std::string name;
	std::vector<std::string> params;
	std::string trailing;
	bool hasTrailing;
	Command(): hasTrailing(false) {}
};

#endif