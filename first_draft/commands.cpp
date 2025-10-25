/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmeniga@student.42vienna.com <tmeniga>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:14:43 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/25 20:52:29 by tmeniga@stu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string server::trim(const std::string& str)
{
	size_t start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
		return ("");
	size_t end = str.find_last_not_of(" \t\n\r\f\v");
	return (str.substr(start, end - start + 1));
}

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
	
	if (password.empty())
	{
		std::cout << "Client#" << client.fd << " -> PASS \n";
		std::string error = E461(client.nickname, "PASS");
		send(client.fd, error.c_str(), error.length(), 0);
		return (1);
	}
	if (password == password_)
	{
		//CORRECT PASSWORD
		client.hasPassword = true;
		std::cout << "Client#" << client.fd << " -> PASS ****\n";
		std::string success = S464(client.nickname);
		send(client.fd, success.c_str(), success.length(), 0);
	}
	else
	{
		//WRONG PASSWORD
		std::cout << "Client#" << client.fd << " -> PASS " << password << "\n";
		if (maxAttemptsReached(client) == true)
		{
			std::string disconnect_msg = E4642(client.nickname);
			send(client.fd, disconnect_msg.c_str(), disconnect_msg.length(), 0);
			return (-1);
		}
		else
		{
			std::string error = E464(client.nickname);
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

void server::setNick(Client &client, std::istringstream &iss)
{
	std::string nickname;
	iss >> nickname;

	std::cout << "Client#" << client.fd << " -> NICK " << nickname << std::endl;
	if (nickname.empty())
	{
		std::string error = E431(client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	if (client.hasPassword == true)
	{
		//CAN SET NICKNAME - PASSWORD VALID
		if (isNameTaken(&Client::nickname, nickname) == true)
		{
			std::string error = E433(nickname);
			send(client.fd, error.c_str(), error.length(), 0);
		}
		else
		{
			client.nickname = nickname;
			client.hasNick = true;
			std::string success = S433(client.nickname);
			send(client.fd, success.c_str(), success.length(), 0);
		}
	}
	else
	{
		//CANNOT SET NICKNAME - PASSWORD INVALID
		std::string error = E4643(client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
	}
}

int server::quit(Client &client, std::istringstream &iss)
{
	std::string reason;
	iss >> reason;

	//DISCONNECT CLIENT
	std::cout << "Client#" << client.fd << " -> QUIT " << reason << std::endl;
	std::string success = S410(client.nickname);
	send(client.fd, success.c_str(), success.length(), 0);
	return (-1);
}

void server::setUser(Client &client, std::istringstream &iss)
{
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);
	realname = trim(realname);
	
	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		std::cout << "Client#" << client.fd << " -> USER " << username << " " << hostname << " " << servername << " " << realname << "\n";
		std::string error = E461(client.nickname, "USER");
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);
	client.username = username;
	client.realname = realname;
	client.hasUser = true;
	std::cout << "Client#" << client.fd << " -> USER " << username << " " << hostname << " " << servername << " " << realname << std::endl;
	
	//USERNAME SET
	std::string success = S411(client.nickname, username);
	send(client.fd, success.c_str(), success.length(), 0);
	
	//REALNAME SET
	success = S412(client.nickname, realname);
	send(client.fd, success.c_str(), success.length(), 0);
}

std::string	server::getStartDate()
{
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);
	
	std::stringstream ss;
	ss << std::setfill('0');
	ss << std::setw(2) << localTime->tm_hour << ":"
		<< std::setw(2) << localTime->tm_min << " "
		<< std::setw(2) << localTime->tm_mday << "/"
		<< std::setw(2) << (localTime->tm_mon + 1) << "/"
		<< (localTime->tm_year + 1900);
	
	return (ss.str());
}

void	server::sendWelcome(Client &client)
{
	std::string welcome = RPL_WELCOME(client.nickname, SERVER_NAME, client.username, client.hostname);
	std::string yourhost = RPL_YOURHOST(client.nickname, SERVER_NAME, VERSION);
	std::string created = RPL_CREATED(client.nickname, startDate);
	std::string myinfo = RPL_MYINFO(client.nickname, SERVER_NAME, VERSION);
	send(client.fd, welcome.c_str(), welcome.length(), 0);
	send(client.fd, yourhost.c_str(), yourhost.length(), 0);
	send(client.fd, created.c_str(), created.length(), 0);
	send(client.fd, myinfo.c_str(), myinfo.length(), 0);
}

void	server::checkRegistration(Client &client)
{
	if (client.hasPassword && client.hasNick && client.hasUser && !client.isRegistered)
	{
		client.isRegistered = true;
		std::cout << "Client#" << client.fd << "'s (" << client.nickname << ") registration successful\n";
		sendWelcome(client);
	}
}

void server::sendPrivate(Client &client, std::istringstream &iss)
{
	std::string target_nick;
	iss >> target_nick;
	std::string message;
	std::getline(iss, message);
	message = trim(message);
	int target_fd = -1;

	std::cout << "Client#" << client.fd << " -> PRIVMSG " << target_nick << message << std::endl;
	for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (it->second.nickname == target_nick)
			target_fd = it->second.fd;
	}
	if (target_fd != -1 && !message.empty())
	{
		//VALID TARGET ANDD MESSAGE
		std::string new_message = ":" + client.nickname + " PRIVMSG " + target_nick + " :" + message + "\n";
		send(target_fd, new_message.c_str(), new_message.length(), 0);
		std::string success = S465(client.nickname);
		send(client.fd, success.c_str(), success.length(), 0);
	}
	else if (target_fd != -1 && message.empty())
	{
		//NO TARGET
		std::string error = E412(client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
	}
	else
	{
		//NO MESSAGE
		std::string error = E401(client.nickname, target_nick);
		send(client.fd, error.c_str(), error.length(), 0);
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
		std::string error = E463(client.nickname, cmd);
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
