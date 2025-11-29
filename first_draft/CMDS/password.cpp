
#include "../server.hpp"

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
	
	if (client.hasPassword)
		return 1;
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS " << password << "\n";
		std::string error = E462(std::string(SERV), client.nickname);
		ft_send(client.fd, error);
		return (1);
	}
	else if (password.empty())
	{
		//INVALID - EMPTY PASSWORD
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS \n";
		std::string error = E461(std::string(SERV), client.nickname, "PASS");
		ft_send(client.fd, error);
		return (1);
	}
	else if (password == password_)
	{
		//CORRECT PASSWORD
		client.hasPassword = true;
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS ****\n";
		std::string success = S467(std::string(SERV), client.nickname);
		ft_send(client.fd, success);
	}
	else
	{
		//WRONG PASSWORD
		std::cout << formatDate() << "Client#" << client.fd << " -> PASS " << password << "\n";
		if (maxAttemptsReached(client) == true)
		{
			std::string disconnect_msg = E4642(std::string(SERV), client.nickname);
			ft_send(client.fd, disconnect_msg);
			close(client.fd);
			std::cout << formatDate() << "Client#" << client.fd << " disconnected\n";
			return (-1);
		}
		else
		{
			std::string error = E464(std::string(SERV), client.nickname);
			ft_send(client.fd, error);
		}
	}
	return (0);
}
