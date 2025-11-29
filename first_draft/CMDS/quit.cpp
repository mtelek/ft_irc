#include "../server.hpp"

int server::quit(Client &client, std::istringstream &iss)
{
	std::string reason;
	iss >> reason;

	//DISCONNECT CLIENT
	std::cout << formatDate() << "Client#" << client.fd << " -> QUIT " << reason << std::endl;
	std::cout << formatDate() << "Client#" << client.fd << " disconnected\n";
	std::string success = S410(std::string(SERV), client.nickname);
	ft_send(client.fd, success);
	close(client.fd);
	return (-1);
}