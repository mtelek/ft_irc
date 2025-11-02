#include "../server.hpp"

int		server::addUser(int fd, Channel& channel)
{
	Client& client = clients_[fd];

	//# ALREADY MEMBER
	if(channel.members.count(fd))
	{
		std::cout << client.nickname << "is already part of " << channel.name << std::endl;  //! SEND TO CLIENT
		return (-1);
	}

	//# +i (invite-only)
	if (channel.isInviteOnly && !channel.invited.count(client.nickname))
	{
		std::cout << client.nickname << "is invite only and user: " << channel.name << "is not invited" << std::endl;	//! SEND TO CLIENT
		return (-1);
	}

	//# +l (user limit)
	if (channel.userLimit > 0 && (int)channel.members.size() >= channel.userLimit)
	{
		std::cout << "channel " << channel.name << " is full: " << client.nickname << "cant join" << std::endl;	//! SEND TO CLIENT
		return (-1);
	}

	//# +k (key)
	if (!channel.key.empty() && channel.key != client.userKey)
	{
		std::cout << client.nickname << "provided wrong key for " << channel.name << std::endl; //! SEND TO CLIENT
		return (-1);
	}

	//# JOIN
	channel.members.insert(fd);
	client.joinedChannels.insert(channel.name);

	//# FIRST MEMBER BECOMES OPERATOR
	if (channel.operators.empty())
		channel.operators.insert(fd);

	//# IF INVITED, DELETE INVITATION
	if (channel.invited.count(client.nickname))
		channel.invited.erase(client.nickname);

	std::cout << client.nickname << " joined " << channel.name << std::endl;
	return (0);

}

int		server::initChannel(int fd, std::string& name)
{
	if (name.empty() || name[0] != '#' || name.size() < 2 || name.size() > 50 ||
		name.find(' ') != std::string::npos || name.find(',') != std::string::npos)
	{
		std::cout << "error invalid channel name" << std::endl;  //! SEND TO CLIENT
		return (-1);
	}

	if (channels_.count(name))
	{
		std::cout << "error channel already exists" << std::endl;  //! SEND TO CLIENT
		return (-1);
	}

	Channel newChannel;
	newChannel.name				= name;
	newChannel.topic			= "";
	newChannel.isInviteOnly		= false;
	newChannel.isTopicLocked	= false;
	newChannel.userLimit		= 0;
	newChannel.key				= "";

	newChannel.members.insert(fd);
	newChannel.operators.insert(fd);

	channels_[name] = newChannel;
	clients_[fd].joinedChannels.insert(name); 

	std::cout << "created channel " << name << " by user " << clients_[fd].nickname << std::endl;
	return (0);
}


int		server::join(Client &client, std::istringstream &iss)
{
	if (!client.isRegistered)
	{
		std::cout << "user not registered" << std::endl;	//! SEND TO CLIENT
		return (-1);
	}
	
	std::string name;
	std::string key;
	std::string extra;
	iss >> name >> key >> extra;

	if (name.empty())
	{
		std::cout << "Error: JOIN requires at least one argument (#channel)" << std::endl;		//! SEND TO CLIENT
		return (-1);
	}

	if (!extra.empty())
	{
		std::cout << "Error: Too many parameters for JOIN Command" << std::endl; 		//! SEND TO CLIENT
		std::cout << "correct usage: JOIN <#channel> <(optional)password>" << std::endl; //! SEND TO CLIENT
		return (-1);
	}

	client.userKey = key;

	std::map<std::string, Channel>::iterator it = channels_.find(name);

	if (it == channels_.end())
	{
		if(initChannel(client.fd, name) == -1)
			return (-1);

		if (!key.empty())
			channels_[name].key = key;

		return (0);
	}
	else
	{
		Channel& channel = it->second;	
		addUser(client.fd, channel);
	}
	return (0);
}