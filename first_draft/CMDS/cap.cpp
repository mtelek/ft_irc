#include "../server.hpp"

int server::cap(Client &client, std::istringstream &iss)
{
	std::string subcmd;
	iss >> subcmd;
	
	if (subcmd == "LS")
	{
		//SEND MINIMAL CAPABILITIES
		std::string response = "CAP * LS :\r\n";
		ft_send(client.fd, response);
	}
	else if (subcmd == "REQ")
	{
		//ACKNOWLEDGE ANY REQUESTED CAPABILITIES
		std::string caps;
		std::getline(iss, caps);
		std::string response = "CAP * ACK " + caps + "\r\n";
		ft_send(client.fd, response);
	}
	else if (subcmd == "END")
	{}
	return (0);
}
