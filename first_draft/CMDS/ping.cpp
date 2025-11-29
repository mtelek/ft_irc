#include "../server.hpp"

int		server::ping(Client &client, std::istringstream &iss)
{
	std::string message;
	std::getline(iss, message);

	if (!message.empty() && message[0] == ' ')
		message.erase(0,1);

	if (!message.empty() && message[0] != ':')
		message = ":" + message;

	std::string out_msg = "PONG " + message + "\r\n";
	ft_send(client.fd, out_msg);
	return (0);
}

