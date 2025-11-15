#include "../server.hpp"

int		server::invite(Client &client, std::istringstream &iss)
{
	if (!client.isRegistered)
	{
		std::cout << "user not registered" << std::endl;	//! SEND TO CLIENT
		return (-1);
	}

	std::string	targetName;
	std::string	channelName;
	std::string extra;

	iss >> targetName >> channelName >> extra;

	if (channelName.empty() || targetName.empty())
	{
		std::cout << "Error: INVITE requires at least 2 arguments: <#channel> <user>" << std::endl; //! SEND TO CLIENT (ERR_NEEDMOREPARAMS)
		return (-1);
	}
	
	if (targetName.find(',') != std::string::npos)
	{
		std::cout << "Error: multiple users not supported in INVITE" << std::endl; //! SEND TO CLIENT
		return (-1);
	}

	if (channelName.find(',') != std::string::npos)
	{
		std::cout << "Error: multiple channels not supported in INVITE" << std::endl; //! SEND TO CLIENT (ERR_TOOMANYTARGETS)
		return (-1);
	}

	if (!extra.empty())
	{
		std::cout << "Error: too many parameters for INVITE command" << std::endl; //! SEND TO CLIENT
		std::cout << "correct usage: INVITE <nickname> <#channel>" << std::endl;  //! SEND TO CLIENT
		return (-1);
	}

	std::map<std::string, Channel>::iterator it = channels_.find(channelName);
	if (it == channels_.end())
	{
		std::cout << "Error: no such channel: " << channelName << std::endl; //! SEND TO CLIENT (ERR_NOSUCHCHANNEL)
		return (-1);
	}
	Channel &channel = it->second;

	if (channel.members.count(client.fd) == 0)
	{
		std::cout << "Error: you are no member of: " << channelName << std::endl; //! SEND TO CLIENT 
		return (-1);
	}

	if (channel.operators.count(client.fd) == 0)
	{
		std::cout << "Error: you are no operator of: " << channelName << std::endl; //! SEND TO CLIENT
		return (-1);
	}

	int target_fd = findClientByNick(targetName);
	if (target_fd == -1)
	{
		std::cout << "Error: no such nick: " << targetName << std::endl; //! SEND TO CLIENT (ERR_NOSUCHNICK)
		return (-1);
	}

	if (channel.members.count(target_fd))
	{
		std::cout << "Error: " << targetName << "  already member of " << std::endl; //! SEND TO CLIENT 
		return (-1);
	}

	std::string out_msg;

	out_msg = ":" + client.nickname + " INVITE " + targetName + " " + channelName + "\r\n";

	if (ft_send(target_fd, out_msg) == -1)
	{
		//! close client 
		return (-1);
	}
	return (0);
}