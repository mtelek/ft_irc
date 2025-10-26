/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_message.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:22:22 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 21:08:23 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::sendPrivate(Client &client, std::istringstream &iss)
{
	std::string target_nick;
	iss >> target_nick;
	std::string message;
	std::getline(iss, message);
	message = trim(message);
	int target_fd = -1;

	std::cout << formatDate() << "Client#" << client.fd << " -> PRIVMSG " << target_nick << " "<< message << std::endl;
	std::string lowerTargetNick = toLowerString(target_nick);
	for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		//LOOK FOR THE FD OF THE TARGET
		if (toLowerString(it->second.nickname) == lowerTargetNick)
		{
			target_fd = it->second.fd;
			break;
		}
	}
	if (target_fd != -1 && !message.empty())
	{
		//VALID TARGET AND MESSAGE
		std::string new_message = ":" + client.nickname + " PRIVMSG " + target_nick + " :" + message + "\r\n";
		send(target_fd, new_message.c_str(), new_message.length(), MSG_DONTWAIT);
		std::string success = S465(std::string(SERV), client.nickname);
		send(client.fd, success.c_str(), success.length(), MSG_DONTWAIT);
	}
	else if (target_fd != -1 && message.empty())
	{
		//NO MESSAGE
		std::string error = E412(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
	}
	else
	{
		//NO TARGET
		std::string error = E401(std::string(SERV), client.nickname, target_nick);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
	}
}
