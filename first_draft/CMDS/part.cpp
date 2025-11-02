#include "../server.hpp"

int		server::part(Client &client, std::istringstream &iss)
{
	if (!client.isRegistered)
	{
		std::cout << "user not registered" << std::endl;	//! SEND TO CLIENT
		return (-1);
	}

	std::string name;
	iss >> name;

	if (name.empty())
	{
		std::cout << "Error: Too few parameters for PART Command" << std::endl; 		//! SEND TO CLIENT
		return (-1);
	}

	if (name.find(',') != std::string::npos)
	{
        std::cout << "Error: multiple channels not supported\n" << std::endl;            //! SEND TO CLIENT
        return (-1);
    }

	//# PARSE REASON
	std::string reason;
	std::getline(iss, reason);
	while (!reason.empty() && reason[0] == ' ')
		reason.erase(0, 1);
	if (!reason.empty() && reason[0] == ':')
		reason.erase(0, 1);
	while (!reason.empty() && reason[0] == ' ')
    	reason.erase(0, 1);

	//# FIND CHANNEL
	std::map<std::string, Channel>::iterator it = channels_.find(name);
	if (it == channels_.end())
	{
		std::cout << "Error: channel not found" << std::endl;		//! SEND TO CLIENT
		return (-1);
	}
	Channel& channel = it->second;	

	//# VERIFY MEMBERSHIP
	if (channel.members.count(client.fd) == 0)
	{
		std::cout << "Error: your not part of the channel" << std::endl;		//! SEND TO CLIENT
		return (-1);
	}

	//# SEND REASON TO CHANNEL

		//! send 

	//# REMOVE FROM CHANNEL
	channel.members.erase(client.fd);
	channel.operators.erase(client.fd);
	client.joinedChannels.erase(channel.name);

	if (channel.members.empty())
		channels_.erase(it);
	else if (channel.operators.empty())
		channel.operators.insert(*channel.members.begin());

	return (0);
}