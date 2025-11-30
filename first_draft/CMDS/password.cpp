
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
	{
		//INVALID - COORECT PASSWORD ALREADY SENT
		std::string error = ERR_PASS_ALREADY_GIVEN(std::string(SERV), client.nickname);
		ft_send(client.fd, error);
		return (1);
	}
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::string error = E462(std::string(SERV), client.nickname);
		ft_send(client.fd, error);
		return (1);
	}
	else if (password.empty())
	{
		//INVALID - EMPTY PASSWORD
		std::string error = E461(std::string(SERV), client.nickname, "PASS");
		ft_send(client.fd, error);
		return (1);
	}
	else if (password == password_)
	{
		//CORRECT PASSWORD
		client.hasPassword = true;
		std::string success = S467(std::string(SERV), client.nickname);
		ft_send(client.fd, success);
	}
	else
	{
		//WRONG PASSWORD
		if (maxAttemptsReached(client) == true)
		{
			std::string disconnect_msg = E4642(std::string(SERV), client.nickname);
			ft_send(client.fd, disconnect_msg);
			close(client.fd);
			std::cout << formatDate(2) << "Client#" << client.fd << " disconnected\n";
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
