#include "../server.hpp"

int		server::topic(Client &client, std::istringstream &iss)
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
		std::cout << "too few parameters" << std::endl;	//! SEND TO CLIENT
		return (-1);
	}

	if (name.find(',') != std::string::npos)
	{
        std::cout << "Error: multiple channels not supported\n" << std::endl;            //! SEND TO CLIENT
        return (-1);
    }

	//# FIND CHANNEL
	std::map<std::string, Channel>::iterator it = channels_.find(name);
	if (it == channels_.end())
	{
		std::cout << "Error: channel not found " << std::endl;		//! SEND TO CLIENT
		return (-1);
	}
	Channel& channel = it->second;	

	//# VERIFY MEMBERSHIP
	if (channel.members.count(client.fd) == 0)
	{
		std::cout << "Error: your not part of the channel" << std::endl;		//! SEND TO CLIENT
		return (-1);
	}

	std::string newTopic;
	std::getline(iss, newTopic);
	while (!newTopic.empty() && newTopic[0] == ' ')
		newTopic.erase(0, 1);
	if (!newTopic.empty() && newTopic[0] == ':')
		newTopic.erase(0, 1);
	while (!newTopic.empty() && newTopic[0] == ' ')
    	newTopic.erase(0, 1);

	if (newTopic.empty())
	{
		if (channel.topic.empty())
			std::cout << "No topic is set for " << channel.name << std::endl;						//! SEND TO CLIENT
		else
			std::cout << "Topic for " << channel.name << ": " << channel.topic << std::endl;		//! SEND TO CLIENT
		return (0);
	}

	if (channel.isTopicLocked && (channel.operators.count(client.fd) == 0))
	{
		std::cout << "your not channel operator " << std::endl;						//! SEND TO CLIENT
		return (-1);
	}

	channel.topic = newTopic;

	//# send to all members
	std::cout << "Topic for " << channel.name << " changed to: " << newTopic << std::endl;

	return (0);
}