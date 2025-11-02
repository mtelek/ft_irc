#include "../server.hpp"

void server::cap(Client &client, std::istringstream &iss)
{
	std::string subcmd;
	iss >> subcmd;
	
	if (subcmd == "LS")
	{
		// Send minimal capabilities or none
		std::string response = "CAP * LS :\r\n";
		send(client.fd, response.c_str(), response.length(), MSG_DONTWAIT);
	}
	else if (subcmd == "REQ")
	{
		// Acknowledge any requested capabilities
		std::string caps;
		std::getline(iss, caps);
		std::string response = "CAP * ACK " + caps + "\r\n";
		send(client.fd, response.c_str(), response.length(), MSG_DONTWAIT);
	}
	else if (subcmd == "END") {}
}