#include "../server.hpp"

int		server::kick(Client &client, std::istringstream &iss, std::string &cmd)
{
	if (!client.isRegistered)
	{
		std::string err =  ERR_NOTREGISTERED(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}

	std::string	channelName;
	std::string	targetName;
	std::string extra;

	iss >> channelName >> targetName >> extra;

	if (channelName.empty() || targetName.empty())
	{
		std::string err =  ERR_NEEDMOREPARAMS(std::string(SERV), client.nickname, cmd);
		ft_send(client.fd, err);
		return (1);
	}
	
	if (channelName.find(',') != std::string::npos)
	{
		std::string err = ERR_TOOMANYCHAN(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}

	if (targetName.find(',') != std::string::npos)
	{
		std::string err = ERR_TOOMANYTARGETS(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
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
	std::map<std::string, Channel>::iterator it = channels_.find(toLowerString(channelName));
	if (it == channels_.end())
	{
		std::string err =  ERR_NOSUCHCHANNEL(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}
	Channel &channel = it->second;

	//# CLIENT MUST BE MEMBER AND OPERATOR ON THE CHANNEL
	if (channel.members.count(client.fd) == 0)
	{
		std::string err = ERR_NOTONCHANNEL(std::string(SERV), client.nickname, channelName);
		ft_send(client.fd, err);
		return (1);
	}

	if (channel.operators.count(client.fd) == 0)
	{
		std::string err = ERR_CHANOPRIVSNEEDED(std::string(SERV), client.nickname, channelName);
		ft_send(client.fd, err);
		return (1);
	}

	//# TARGET MUST EXISTS AND BE MEMBER OF THE CHANNEL
	int target_fd = findClientByNick(targetName);
	if (target_fd == -1)
	{
		std::string err =  E401(std::string(SERV), client.nickname, targetName);
		ft_send(client.fd, err);
		return (1);
	}

	if (channel.members.count(target_fd) == 0)
	{
		std::string err = ERR_USERNOTINCHANNEL(std::string(SERV), client.nickname, targetName, channel.name);
		ft_send(client.fd, err);
		return (1);
	}

	std::string out_msg;
	out_msg = ":" + client.nickname + " KICK " + channelName + " " + targetName + " :" + comment + "\r\n";
	for  (std::set<int>::iterator m = channel.members.begin(); m != channel.members.end(); m++)
	{
		int member_fd = *m;
		if (clients_.count(member_fd) == 0)		continue;
		
		if (ft_send(member_fd, out_msg) == -1)
			return (-1);
	}
	return (0);
}
