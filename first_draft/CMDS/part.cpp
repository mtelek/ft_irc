#include "../server.hpp"

int		server::part(Client &client, std::istringstream &iss, std::string &cmd)
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