#include "../server.hpp"

int		server::addUser(int fd, Channel& channel)
{
	Client& client = clients_[fd];

	//# ALREADY MEMBER
	if(channel.members.count(fd))
	{
		std::string err = ERR_USERONCHANNEL(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
		return (-1);
	}

	//# +i (invite-only)
	if (channel.isInviteOnly && !channel.invited.count(client.nickname))
	{
		std::string err = ERR_INVITEONLYCHAN(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
		return (-1);
	}

	//# +l (user limit)
	if (channel.userLimit > 0 && (int)channel.members.size() >= channel.userLimit)
	{
		std::string err = ERR_CHANNELISFULL(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
		return (-1);
	}

	//# +k (key)
	if (!channel.key.empty() && channel.key != client.userKey)
	{
		std::string err = ERR_BADCHANNELKEY(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
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

	std::cout << formatDate() << "Client#" << client.fd << "(" << client.nickname << ") joined " << channel.name << std::endl;
	return (0);

}

int		server::initChannel(int fd, std::string& name)
{
	if (name.empty() || name[0] != '#' || name.size() < 2 || name.size() > 50 ||
		name.find(' ') != std::string::npos || name.find(',') != std::string::npos)
	{
		std::string err = ERR_BADCHANMASK(std::string(SERV), name);
		ft_send(fd, err);
		return (-1);
	}

	if (channels_.count(name))
	{
		std::string err = ERR_CHANNELALREADYEXISTS(std::string(SERV), name);
		ft_send(fd, err);
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

	std::cout << formatDate() << "Client#" << fd << "(" << clients_[fd].nickname << ") created channel: " << name << std::endl;
	return (0);
}


int		server::join(Client &client, std::istringstream &iss, std::string &cmd)
{
	if (!client.isRegistered)
	{
		std::string err =  ERR_NOTREGISTERED(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}
	
	std::string name;
	std::string key;
	std::string extra;
	iss >> name >> key >> extra;

	if (name.empty())
	{
		std::string err =  ERR_NEEDMOREPARAMS(std::string(SERV), client.nickname, cmd);
		ft_send(client.fd, err);
		return (-1);
	}

	if (!extra.empty())
	{
		std::string err = ERR_TOOMANYPARAMS_J(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
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