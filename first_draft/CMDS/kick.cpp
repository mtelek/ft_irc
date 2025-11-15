#include "../server.hpp"

int		server::kick(Client &client, std::istringstream &iss)
{
	if (!client.isRegistered)
	{
		std::cout << "user not registered" << std::endl;	//! SEND TO CLIENT
		return (-1);
	}

	std::string	channelName;
	std::string	targetName;
	std::string extra;

	iss >> channelName >> targetName >> extra;

	if (channelName.empty() || targetName.empty())
	{
		std::cout << "Error: KICK requires at least 2 arguments: <#channel> <user>" << std::endl; //! SEND TO CLIENT (ERR_NEEDMOREPARAMS)
		return (-1);
	}
	
	if (channelName.find(',') != std::string::npos)
	{
		std::cout << "Error: multiple channels not supported in KICK" << std::endl; //! SEND TO CLIENT (ERR_TOOMANYTARGETS)
		return (-1);
	}

	if (targetName.find(',') != std::string::npos)
	{
		std::cout << "Error: multiple users not supported in KICK" << std::endl; //! SEND TO CLIENT
		return (-1);
	}

	std::string comment;
	if (!extra.empty())
	{
		comment = extra;
		std::string remainder;
		std::getline(iss, remainder);
		if (!remainder.empty() && remainder[0] == ' ')
			remainder.erase(0,1);
		if (!remainder.empty())
		{
			if (!comment.empty())
				comment += " ";
			comment += remainder;
		}
	}

	if (!comment.empty())
		comment = "Kicked";

	//# channel must exist
	std::map<std::string, Channel>::iterator it = channels_.find(channelName);
	if (it == channels_.end())
	{
		std::cout << "Error: no such channel: " << channelName << std::endl; //! SEND TO CLIENT (ERR_NOSUCHCHANNEL)
		return (-1);
	}
	Channel &channel = it->second;

	//# client must be menber and operator on that channel
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

	//# target must exist and be member in channel
	int target_fd = findClientByNick(targetName);
	if (target_fd == -1)
	{
		std::cout << "Error: no such nick: " << targetName << std::endl; //! SEND TO CLIENT (ERR_NOSUCHNICK)
		return (-1);
	}

	if (channel.members.count(target_fd) == 0)
	{
		std::cout << "Error: no such member: " << targetName << std::endl; //! SEND TO CLIENT 
		return (-1);
	}

	std::string out_msg;

	out_msg = ":" + client.nickname + " KICK " + channelName + " " + targetName + " :" + comment + "\r\n";

	for  (std::set<int>::iterator m = channel.members.begin(); m != channel.members.end(); m++)
	{
		int member_fd = *m;
		if (clients_.count(member_fd) == 0)		continue;
		
		if (ft_send(member_fd, out_msg) == -1)
		{
			//! close client 
			return (-1);
		}
	}
	return (0);
}
