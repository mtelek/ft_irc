#include "../server.hpp"

int server::cap(Client &client, std::istringstream &iss)
{
	std::string subcmd;
	iss >> subcmd;
	
	if (subcmd == "LS")
	{
		// Send minimal capabilities or none
		std::string response = "CAP * LS :\r\n";
		ft_send(client.fd, response);
	}
	else if (subcmd == "REQ")
	{
		// Acknowledge any requested capabilities
		std::string caps;
		std::getline(iss, caps);
		std::string response = "CAP * ACK " + caps + "\r\n";
		ft_send(client.fd, response);
	}
	else if (subcmd == "END")
	{}
	return (0);
}
