/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:14:43 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/21 22:23:20 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool server::maxAttemptsReached(Client &client)
{
	client.wrongPass = client.wrongPass + 1;
	if (client.wrongPass >= MAX_PASS_ATTEMPTS)
		return (true);
	return (false);
}

int server::authenticate(Client &client, std::string &password)
{
	std::cout << "Client " << client.fd << " is trying to connect...\n";
	if (password == password_)
	{
		client.hasPassword = true;
		std::cout << "Client " << client.fd << " authenticated succesfully\n";
		std::string success = "Client has been authenticated\r\n";
		send(client.fd, success.c_str(), success.length(), 0);
	}
	else
	{
		if (maxAttemptsReached(client) == true)
		{
			std::cout << "Error: Too many failed passwort attempts. Client " << client.fd << " disconnected\n";
			std::string disconnect_msg = "ERROR :Too many failed password attempts. Disconnecting.\r\n";
			send(client.fd, disconnect_msg.c_str(), disconnect_msg.length(), 0);
			return (-1);
		}
		else
		{
			std::cout << "Error: Client " << client.fd << " has given an incorrect password\n";
			std::string error = "464: Password incorrect\r\n";
			send(client.fd, error.c_str(), error.length(), 0);
		}
	}
	return (0);
}

bool server::isNameTaken(std::string Client::* member, const std::string& name)
{
	for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (it->second.*member == name)
			return (true);
	}
	return (false);
}

void server::setNick(Client &client, std::string &nickname)
{
	std::cout << "Client " << client.fd << " is trying to set a nickname...\n";
	if (client.hasPassword == true)
	{
		if (isNameTaken(&Client::nickname, nickname) == true)
		{
			std::cout << "Client " << client.fd << " could not set nickname, nickname already in use\n";
			std::string error = std::string("433: ") + nickname + E433 + "\r\n";
			send(client.fd, error.c_str(), error.length(), 0);
		}
		else
		{
			client.nickname = nickname;
			client.hasNick = true;
			std::cout << "Client " << client.fd << " set nickname to " << client.nickname << std::endl;
			std::string success = "Client set nickname to " + nickname + "\r\n";
			send(client.fd, success.c_str(), success.length(), 0);
		}
	}
	else
	{
		std::cout << "Error: Client " << client.fd << " has not been authenticated yet\n";
		std::string error = "Error: you need to be authenticated first\r\n";
		send(client.fd, error.c_str(), error.length(), 0);
	}
}

int server::quit(Client &client, std::string &reason)
{
	std::cout << "Client " << client.fd << " has been left the server\n";
	std::string success = "Disconnection successful\r\n";
	send(client.fd, success.c_str(), success.length(), 0);
	return (-1);
}

void server::setUser(Client &client, std::istringstream &iss) //not done
{
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);
	
	if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);
	client.username = username; //checker here is missing
	client.realname = realname; //also here
	client.hasUser = true;
	std::cout << "Client " << client.fd << " set username to: " << username << std::endl;
	std::cout << "Client " << client.fd << " set realname to: " << realname << std::endl;
}

int server::executeCommands(int client_fd, const std::string& command)
{
	Client& client = clients_[client_fd];

	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	if (cmd == "PASS")
	{
		std::string password;
		iss >> password;
		if (authenticate(client, password) == -1)
			return (-1);
	}
	else if (cmd == "NICK")
	{
		std::string nickname;
		iss >> nickname;
		setNick(client, nickname);
	}
	else if (cmd == "USER")
	{
		setUser(client, iss);
	}
	else if (cmd == "QUIT")
	{
		std::string reason;
		iss >> reason;
		if (quit(client, reason) == -1)
			return (-1);
	}
	else
	{
		std::string error = "Error: Command not found\r\n";
		send(client.fd, error.c_str(), error.length(), 0);
	}
	return (0);
}

int	server::recieveMessage(std::vector<pollfd> fds, size_t i, char *buffer, ssize_t bytes)
{
	buffer[bytes] = '\0';
	Client& client = clients_[fds[i].fd];
	client.buffer += std::string(buffer, bytes);
	
	int j;
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
