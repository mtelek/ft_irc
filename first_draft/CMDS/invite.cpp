#include "../server.hpp"

int		server::invite(Client &client, std::istringstream &iss, std::string &cmd)
{
	if (!client.isRegistered)
	{
		std::string err =  ERR_NOTREGISTERED(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}

	std::string	targetName;
	std::string	channelName;
	std::string extra;

	iss >> targetName >> channelName >> extra;

	if (channelName.empty() || targetName.empty())
	{
		std::string err =  ERR_NEEDMOREPARAMS(std::string(SERV), client.nickname, cmd);
		ft_send(client.fd, err);
		return (-1);
	}
	
	if (targetName.find(',') != std::string::npos)
	{
		std::string err = ERR_TOOMANYTARGETS(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}

	if (channelName.find(',') != std::string::npos)
	{
		std::string err = ERR_TOOMANYCHAN(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}

	if (!extra.empty())
	{
		std::string err = ERR_TOOMANYPARAMS_I(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}

	std::map<std::string, Channel>::iterator it = channels_.find(channelName);
	if (it == channels_.end())
	{
		std::string err = ERR_NOSUCHCHANNEL(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}
	Channel &channel = it->second;

	if (channel.members.count(client.fd) == 0)
	{
		std::string err = ERR_NOTONCHANNEL(std::string(SERV), client.nickname, channelName);
		ft_send(client.fd, err);
		return (-1);
	}

	if (channel.operators.count(client.fd) == 0)
	{
		std::string err = ERR_CHANOPRIVSNEEDED(std::string(SERV), client.nickname, channelName);
		ft_send(client.fd, err);
		return (-1);
	}

	int target_fd = findClientByNick(targetName);
	if (target_fd == -1)
	{
		std::string err = E401(std::string(SERV), client.nickname, targetName);
		ft_send(client.fd, err);
		return (-1);
	}

	if (channel.members.count(target_fd))
	{
		std::string err = ERR_USERONCHANNEL(std::string(SERV), client.nickname, channelName);
		ft_send(client.fd, err);
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
