/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:14:43 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 17:56:15 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int server::quit(Client &client, std::istringstream &iss)
{
	std::string reason;
	iss >> reason;

	//DISCONNECT CLIENT
	std::cout << formatDate() << "Client#" << client.fd << " -> QUIT " << reason << std::endl;
	std::string success = S410(std::string(SERV), client.nickname);
	send(client.fd, success.c_str(), success.length(), 0);
	close(client.fd);
	return (-1);
}

void	server::sendWelcome(Client &client)
{
	std::string welcome = RPL_WELCOME(std::string(SERV), client.nickname ,SERVER_NAME, client.username, client.hostname);
	std::string yourhost = RPL_YOURHOST(std::string(SERV), SERVER_NAME, VERSION);
	std::string created = RPL_CREATED(std::string(SERV), startDate);
	std::string myinfo = RPL_MYINFO(std::string(SERV), SERVER_NAME, VERSION);
	std::string isupport = RPL_ISUPPORT(std::string(SERV));
	send(client.fd, welcome.c_str(), welcome.length(), 0);
	send(client.fd, yourhost.c_str(), yourhost.length(), 0);
	send(client.fd, created.c_str(), created.length(), 0);
	send(client.fd, myinfo.c_str(), myinfo.length(), 0);
	send(client.fd, isupport.c_str(), isupport.length(), 0);
}

void	server::checkRegistration(Client &client)
{
	if (client.hasPassword && client.hasNick && client.hasUser && !client.isRegistered)
	{
		client.isRegistered = true;
		std::cout << formatDate() << "Client#" << client.fd << "'s (" << client.nickname << ") registration successful\n";
		sendWelcome(client);
	}
}

int server::executeCommands(int client_fd, const std::string& command)
{
	Client& client = clients_[client_fd];

	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	if (cmd == "PASS")
	{
		if (authenticate(client, iss) == -1)
			return (-1);
	}
	else if (cmd == "NICK")
		setNick(client, iss);
	else if (cmd == "USER")
		setUser(client, iss);
	else if (cmd == "QUIT")
	{
		if (quit(client, iss) == -1)
			return (-1);
	}
	else if (cmd == "PRIVMSG")
	{
		sendPrivate(client, iss);
	}
	else if (cmd == "JOIN")
	{
		join(client, iss);
	}
	else
	{
		//INVALID COMMAND
		std::string error = E463(std::string(SERV), client.nickname, cmd);
		send(client.fd, error.c_str(), error.length(), 0);
	}
	checkRegistration(client);
	return (0);
}

int	server::recieveMessage(std::vector<pollfd> fds, size_t i, char *buffer, ssize_t bytes)
{
	buffer[bytes] = '\0';
	Client& client = clients_[fds[i].fd];
	client.buffer += std::string(buffer, bytes);
	
	int j = 0;
	size_t pos;
	if ((pos = client.buffer.find("\r\n")) != std::string::npos)
		j = 2;
	else if ((pos = client.buffer.find('\n')) != std::string::npos)
		j = 1;
	if (j)
	{
		std::string message = client.buffer.substr(0, pos);
		client.buffer.erase(0, pos + j);
		if (!message.empty())
		{
			if (executeCommands(fds[i].fd, message) == -1)
				return (-1);
		}
	}
	return (0);
}
