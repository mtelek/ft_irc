/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   password.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:18:56 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 16:19:39 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool server::maxAttemptsReached(Client &client)
{
	client.wrongPass++;
	if (client.wrongPass >= MAX_PASS_ATTEMPTS)
		return (true);
	return (false);
}

int server::authenticate(Client &client, std::istringstream &iss)
{
	std::string password;
	iss >> password;
	
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS " << password << "\n";
		std::string error = E462(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return (1);
	}
	else if (password.empty())
	{
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS \n";
		std::string error = E461(std::string(SERV), client.nickname, "PASS");
		send(client.fd, error.c_str(), error.length(), 0);
		return (1);
	}
	else if (password == password_)
	{
		//CORRECT PASSWORD
		client.hasPassword = true;
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS ****\n";
		std::string success = S464(std::string(SERV), client.nickname);
		send(client.fd, success.c_str(), success.length(), 0);
	}
	else
	{
		//WRONG PASSWORD
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS " << password << "\n";
		if (maxAttemptsReached(client) == true)
		{
			std::string disconnect_msg = E4642(std::string(SERV), client.nickname);
			send(client.fd, disconnect_msg.c_str(), disconnect_msg.length(), 0);
			close(client.fd);
			return (-1);
		}
		else
		{
			std::string error = E464(std::string(SERV), client.nickname);
			send(client.fd, error.c_str(), error.length(), 0);
		}
	}
	return (0);
}
