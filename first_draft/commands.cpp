
#include "server.hpp"

enum CommandType {
    CMD_UNKNOWN = -1,
    CMD_CAP,
    CMD_PASS,
    CMD_NICK,
    CMD_USER,
    CMD_QUIT,
    CMD_PRIVMSG,
    CMD_JOIN,
	CMD_KICK,
	CMD_INVITE,
	CMD_PART,
	CMD_TOPIC,
	CMD_MODE,
};

static CommandType getCommandType(const std::string &cmd)
{
	if (cmd == "CAP")		return CMD_CAP;
    if (cmd == "PASS")		return CMD_PASS;
    if (cmd == "NICK")		return CMD_NICK;
    if (cmd == "USER")		return CMD_USER;
    if (cmd == "QUIT")		return CMD_QUIT;
    if (cmd == "PRIVMSG")	return CMD_PRIVMSG;
    if (cmd == "JOIN")		return CMD_JOIN;
    if (cmd == "KICK")		return CMD_KICK;
    if (cmd == "INVITE")	return CMD_INVITE;
    if (cmd == "PART")		return CMD_PART;
    if (cmd == "TOPIC")		return CMD_TOPIC;
    if (cmd == "MODE")		return CMD_MODE;
    return CMD_UNKNOWN;
}

int server::executeCommands(int client_fd, const std::string& command)
{
	Client& client = clients_[client_fd];

	std::cout << command << std::endl;

	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	CommandType type = getCommandType(cmd);

	switch(type)
	{
		case CMD_CAP:			//! UNTESTED
			cap(client, iss);
			break;
		
		case CMD_PASS:			//! UNTESTED
            if (authenticate(client, iss) == -1)
			{
				std::cout << "AAAAAAAA" << std::endl;
                return (-1);
			}
            break;

		case CMD_NICK:			//! UNTESTED 
            setNick(client, iss);
            break;

        case CMD_USER:			//! UNTESTED
            setUser(client, iss);
            break;

        case CMD_QUIT:			//! UNTESTED
            if (quit(client, iss) == -1)
                return (-1);
            break;

        case CMD_PRIVMSG:		//! UNTESTED
            sendPrivate(client, iss);
            break;

        case CMD_JOIN:			//! UNTESTED
            join(client, iss);
            break;

		case CMD_KICK:		//! UNTESTED
			kick(client, iss);
			break;

		case CMD_INVITE:		//! UNTESTED
			invite(client, iss);
			break;

		case CMD_PART:			//! UNTESTED
			part(client, iss);
			break;

		case CMD_TOPIC: 	//! UNTESTED
			topic(client, iss);
			break;

		case CMD_MODE:		//! UNTESTED
			mode(client, iss);
		break;

		default:
		{
			std::string error = E463(std::string(SERV), client.nickname, cmd);
			send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		}
	}

	checkRegistration(client);
	return (0);
}

int	server::recieveMessage(std::vector<pollfd> fds, size_t i, char *buffer, ssize_t bytes)
{
	buffer[bytes] = '\0';
	Client& client = clients_[fds[i].fd];
	client.buffer.append(buffer, bytes);

	while (true)
	{
		size_t	pos;
		int		len = 0;

		if ((pos = client.buffer.find("\r\n")) != std::string::npos)
			len = 2;
		else if	((pos = client.buffer.find('\n')) != std::string::npos)  //! dk if need to keep this, some clients sometimes only send \n instead of \r\n
			len = 1;

		if (!len)
			break;
		
		std::string command = client.buffer.substr(0, pos);
		client.buffer.erase(0, pos + len);

		if (!command.empty())
		{
			std::cout << command << std::endl;  //for debugging
			if (executeCommands(fds[i].fd, command) == -1)
                return (-1); // tell caller to close client
		}
	}
	return (0);
}	
	// int j = 0;
	// size_t pos;
	// if ((pos = client.buffer.find("\r\n")) != std::string::npos)
	// 	j = 2;
	// else if ((pos = client.buffer.find('\n')) != std::string::npos)
	// 	j = 1;
	// if (j)
	// {
	// 	std::string message = client.buffer.substr(0, pos);
	// 	client.buffer.erase(0, pos + j);
	// 	if (!message.empty())
	// 	{
	// 		if (executeCommands(fds[i].fd, message) == -1)
	// 			return (-1);
	// 	}
	// }
	// return (0);
// }
