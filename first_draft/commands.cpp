/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmeniga@student.42vienna.com <tmeniga>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:14:43 by mtelek            #+#    #+#             */
/*   Updated: 2025/11/02 15:54:42 by tmeniga@stu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	server::sendWelcome(Client &client)
{
	std::string welcome = RPL_WELCOME(std::string(SERV), client.nickname ,SERVER_NAME, client.username, client.hostname);
	std::string yourhost = RPL_YOURHOST(std::string(SERV), SERVER_NAME, VERSION);
	std::string created = RPL_CREATED(std::string(SERV), startDate);
	std::string myinfo = RPL_MYINFO(std::string(SERV), SERVER_NAME, VERSION);
	std::string isupport = RPL_ISUPPORT(std::string(SERV));
	send(client.fd, welcome.c_str(), welcome.length(), MSG_DONTWAIT);
	send(client.fd, yourhost.c_str(), yourhost.length(), MSG_DONTWAIT);
	send(client.fd, created.c_str(), created.length(), MSG_DONTWAIT);
	send(client.fd, myinfo.c_str(), myinfo.length(), MSG_DONTWAIT);
	send(client.fd, isupport.c_str(), isupport.length(), MSG_DONTWAIT);
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

	if (cmd == "CAP")
	{
		cap(client, iss);
	}
	else if (cmd == "PASS")
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
		//commented out to test irssi till join isnt ready
		join(client, iss);
	}
	else
	{
		//INVALID COMMAND
		std::string error = E463(std::string(SERV), client.nickname, cmd);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
	}
	checkRegistration(client);
	client.buffer.clear();
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
