#include "server.hpp"




// int		server::addUser(int fd, Channel& channel)
// {
// 	//check if already meber of channel








// }


int		server::initChannel(int fd, std::string& name)
{
	if (name.empty() || name[0] != '#')
	{
		std::cout << "error invalid channel name" << std::endl;
		return (-1);
	}

	if (channels_.count(name))
	{
		std::cout << "error channel already exists" << std::endl;
		return (-1);
	}

	Channel newChannel;
	newChannel.name				= name;
	newChannel.topic			= "";	     //maybe delete bc unnecessery
	newChannel.isInviteOnly		= false;
	newChannel.isTopicLocked	= false;
	newChannel.userLimit		= 0;
	newChannel.key				= "";

	newChannel.members.insert(fd);
	newChannel.members.insert(fd);

	channels_[name] = newChannel;
	clients_[fd].joinedChannels.insert(name); 

	std::cout << "created channel " << name << "by user " << clients_[fd].nickname << std::endl;
	return (0);
}


int		server::join(Client &client, std::istringstream &iss)
{
	std::string name;
	iss >> name;

	std::map<std::string, Channel>::iterator it = channels_.find(name);

	if (it == channels_.end())
	{
		if(initChannel(client.fd, name) == -1)
			return (-1);
		return (0);
	}
	else
	{
		//Channel& channel = it->second;	
		//addUser(client.fd, channel);
		return (0);
	}


	
	//# 1 look up if the channel exist
	//# -> if channel doesnt exist, initChannel();
	//# -> else join channel

	return (0);
}
