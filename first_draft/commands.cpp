/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:14:43 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 16:00:58 by mtelek           ###   ########.fr       */
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

std::string server::formatDate()
{
	std::string date = "[" + getStartDate() + "] ";
	return (date);
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

std::string server::toLowerString(const std::string& str)
{
	std::string lower = str;
	for (std::string::iterator it = lower.begin(); it != lower.end(); ++it)
		*it = std::tolower(*it);
	return (lower);
}

bool server::isNameTaken(std::string Client::* member, const std::string& name)
{
	std::string lowerName = toLowerString(name);
	for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (toLowerString(it->second.*member) == lowerName)
			return (true);
	}
	return (false);
}

bool server::isValidName(Client &client, std::string &nickname)
{
	if (nickname.empty())
	{
		//NO NICKNAME
		std::string error = E431(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return (false);
	}
	const std::string forbiddenChars = " ,*?!@";
	for (size_t i = 0; i < nickname.length(); ++i)
	{
		//FORBIDDEN CHARACTERS
		if (forbiddenChars.find(nickname[i]) != std::string::npos)
		{
			std::string error = E432(std::string(SERV), client.nickname);
			send(client.fd, error.c_str(), error.length(), 0);
			return (false);
		}
	}
	if (nickname[0] == '$' || nickname[0] == ':')
	{
		//FORBIDDEN CHARACTERS
		std::string error = E432(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return (false);
	}
	const std::string Prefixes = "#&@+"; // or just only &
	if (Prefixes.find(nickname[0]) != std::string::npos)
	{
		//FORBIDDEN PREFIXES
		std::string error = E432(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return (false);
	}
	if (nickname.find('.') != std::string::npos)
	{
		//WARNING - DOT SHOULDNT BE USED
		std::string warning = E4322(std::string(SERV), client.nickname);
		send(client.fd, warning.c_str(), warning.length(), 0);
		return (true);
	}
	return (true);
}

void server::setNick(Client &client, std::istringstream &iss)
{
	std::string nickname;
	iss >> nickname;

	std::cout << formatDate() << "Client#" << client.fd << " -> NICK " << nickname << std::endl;
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::string error = E462(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	else if (client.hasPassword == true)
	{
		//CAN SET NICKNAME - PASSWORD VALID
		if (isNameTaken(&Client::nickname, nickname) == true)
		{
			std::string error = E433(std::string(SERV), client.nickname);
			send(client.fd, error.c_str(), error.length(), 0);
		}
		else if (isValidName(client, nickname) == false)
			return ;
		else
		{
			client.nickname = nickname;
			client.hasNick = true;
			std::string success = S433(std::string(SERV), client.nickname);
			send(client.fd, success.c_str(), success.length(), 0);
		}
	}
	else
	{
		//CANNOT SET NICKNAME - PASSWORD INVALID
		std::string error = E4643(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
	}
}

int server::quit(Client &client, std::istringstream &iss)
{
	std::string reason;
	iss >> reason;

	//DISCONNECT CLIENT
	std::cout << formatDate() << "Client#" << client.fd << " -> QUIT " << reason << std::endl;
	std::string success = S410(std::string(SERV), client.nickname);
	send(client.fd, success.c_str(), success.length(), 0);
	close(client.fd); //added this here
	return (-1);
}

void server::setUser(Client &client, std::istringstream &iss)
{
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);
	realname = trim(realname);
	
	std::cout << formatDate() << "Client#" << client.fd << " -> USER " << username << " " << hostname << " " << servername << " " << realname << "\n";
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::string error = E462(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		//INVALID - EMPTY ARGUMENTS
		std::string error = E461(std::string(SERV), client.nickname, "USER");
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	else if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);
	client.username = username;
	client.realname = realname;
	client.hasUser = true;
	
	//USERNAME SET
	std::string success = S411(std::string(SERV), client.nickname, username);
	send(client.fd, success.c_str(), success.length(), 0);
	
	//REALNAME SET
	success = S412(std::string(SERV), client.nickname, realname);
	send(client.fd, success.c_str(), success.length(), 0);
}

std::string	server::getStartDate()
{
	//GETTING THE START DATE OF THE SERVER OR THE SERVER MESSAGE
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);
	
	std::stringstream ss;
	ss << std::setfill('0');
	ss << std::setw(2) << localTime->tm_hour << ":"
		<< std::setw(2) << localTime->tm_min << ":"
		<< std::setw(2) << localTime->tm_sec << " "
		<< std::setw(2) << localTime->tm_mday << "/"
		<< std::setw(2) << (localTime->tm_mon + 1) << "/"
		<< (localTime->tm_year + 1900);
	
	return (ss.str());
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
			target_fd = it->second.fd;
	}
	if (target_fd != -1 && !message.empty())
	{
		//VALID TARGET ANDD MESSAGE
		std::string new_message = ":" + client.nickname + " PRIVMSG " + target_nick + " :" + message + "\n";
		send(target_fd, new_message.c_str(), new_message.length(), 0);
		std::string success = S465(std::string(SERV), client.nickname);
		send(client.fd, success.c_str(), success.length(), 0);
	}
	else if (target_fd != -1 && message.empty())
	{
		//NO TARGET
		std::string error = E412(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
	}
	else
	{
		//NO MESSAGE
		std::string error = E401(std::string(SERV), client.nickname, target_nick);
		send(client.fd, error.c_str(), error.length(), 0);
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
