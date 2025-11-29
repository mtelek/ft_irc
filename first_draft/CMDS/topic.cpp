#include "../server.hpp"

int		server::topic(Client &client, std::istringstream &iss, std::string &cmd)
{
	if (!client.isRegistered)
	{
		std::string err =  ERR_NOTREGISTERED(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}

	std::string name;
	iss >> name;
	if (name.empty())
	{
		std::string err =  ERR_NEEDMOREPARAMS(std::string(SERV), client.nickname, cmd);
		ft_send(client.fd, err);
		return (-1);
	}

	if (name.find(',') != std::string::npos)
	{
        std::string err = ERR_TOOMANYCHAN(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
        return (-1);
    }

	//# FIND CHANNEL
	std::map<std::string, Channel>::iterator it = channels_.find(toLowerString(name));
	if (it == channels_.end())
	{
		std::string err = ERR_NOSUCHCHANNEL(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}
	Channel& channel = it->second;	

	//# VERIFY MEMBERSHIP
	if (channel.members.count(client.fd) == 0)
	{
		std::string err = ERR_NOTONCHANNEL(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
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
		std::string err;
		if (channel.topic.empty())
			err = RPL_NOTOPIC(std::string(SERV), client.nickname, channel.name);
		else
			err = RPL_TOPIC(std::string(SERV), client.nickname, channel.name, channel.topic);
		ft_send(client.fd, err);
		return (0);
	}

	if (channel.isTopicLocked && (channel.operators.count(client.fd) == 0))
	{
		std::string err = ERR_CHANOPRIVSNEEDED(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
		return (-1);
	}
	channel.topic = newTopic;
	std::string message = "Topic for " + channel.name + " changed to: " + newTopic + "\r\n";
	sendToAllChannelMembers(channel, message);
	return (0);
}