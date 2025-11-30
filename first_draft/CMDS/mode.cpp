#include "../server.hpp"

int		server::mode(Client &client, std::istringstream &iss, std::string &cmd)
{
	if (!client.isRegistered)
	{
		std::string err =  ERR_NOTREGISTERED(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}

	std::string name; 
	if (!(iss >> name) || name.empty())
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

	std::map<std::string, Channel>::iterator it = channels_.find(toLowerString(name));
	if (it == channels_.end())
	{
		std::string err = ERR_NOSUCHCHANNEL(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}
	Channel& channel = it->second;

	if (channel.members.count(client.fd) == 0)
	{
		std::string err = ERR_NOTONCHANNEL(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
		return (-1);
	}

	std::string token;
	iss >> token;
	if (token.empty())
	{
		std::string modes = getModes(channel);
		if (ft_send(client.fd, modes) == -1)
			return (-1);
		return (0);
	}

	if (channel.operators.count(client.fd) == 0) 
	{
		std::string err = ERR_CHANOPRIVSNEEDED(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
		return (-1);
	}

	std::vector<std::string> modes;
	std::vector<std::string> params;

	modes.push_back(token);

	while (iss >> token)	
	{
		if (token[0] == '+' || token[0] == '-')	
			modes.push_back(token);
		else
			params.push_back(token);
	}

	char	sign = 0;
	int		paramsUsed = 0;

	for (int i = 0; i < (int)modes.size(); i++)
	{
		const std::string t = modes[i];
		if (t.empty())	continue;

		if (t[0] == '+' || t[0] == '-')
			sign = t[0];
		else
			continue;

		for (int j = 1; j < (int)t.size(); j++)
		{
			const char c = t[j];
			switch (c)
			{
				case 'i' :
					channel.isInviteOnly = (sign == '+');
					break;

				case 't' :
					channel.isTopicLocked = (sign == '+');
					break;

				case 'k' :
					if (sign == '+')
					{
						if (!params.size() || paramsUsed >= (int)params.size())
						{
							std::string err = ERR_NEEDMOREPARAMS(std::string(SERV), client.nickname, "MODE");
							ft_send(client.fd, err);
							return (-1);
						}
						channel.key = params[paramsUsed];
						paramsUsed++;
					}
					else
					{
						channel.key = "";
					}
					break;
				
				case 'l' :
					if (sign == '+')
					{
						if (!params.size() || paramsUsed >= (int)params.size())
						{
							std::string err = ERR_NEEDMOREPARAMS(std::string(SERV), client.nickname, "MODE");
							ft_send(client.fd, err);
							return (-1);
						}

						if (!isNumber(trim(params[paramsUsed])))
						{
							std::string err = ERR_INVALIDMODEPARAM(std::string(SERV), client.nickname, channel.name, "l", params[paramsUsed]);
							ft_send(client.fd, err);
							return (-1);
						}
						int lim = std::atoi(params[paramsUsed].c_str());
						paramsUsed++;
						if (lim < 0) lim = 0;
						channel.userLimit = lim;
					}
					else
					{
						channel.userLimit = 0;
					}
					break;

				case 'o' :
				{		
						if (!params.size() || paramsUsed >= (int)params.size())
						{
							std::string err = ERR_NEEDMOREPARAMS(std::string(SERV), client.nickname, "MODE");
							ft_send(client.fd, err);
							return (-1);
						}

						if (channel.operators.count(client.fd) == 0) 
						{
							std::string err = ERR_CHANOPRIVSNEEDED(std::string(SERV), client.nickname, channel.name);
							ft_send(client.fd, err);
							return (-1);
						}

						std::string targetNick = params[paramsUsed];
						paramsUsed++;

						int targetFd = findClientByNick(targetNick);
						if (targetFd == -1)
						{
							std::string err = E401(std::string(SERV), client.nickname, targetNick);
							ft_send(client.fd, err);
							return (-1);
						}

						if (channel.members.count(targetFd) == 0)
						{
							std::string err = ERR_NOTONCHANNEL(std::string(SERV), client.nickname, channel.name);
							ft_send(client.fd, err);
							return (-1);
						}

						if (sign == '+')
						{
							if (client.fd == targetFd)
							{
								std::string err = ERR_CHANOPRIVSNEEDED(std::string(SERV), client.nickname, channel.name);
								ft_send(client.fd, err);
								return (-1);
							}

							if (channel.operators.count(targetFd))
							{
								std::string err = ERR_USERISOPERATOR(std::string(SERV), client.nickname, channel.name, targetNick);
								ft_send(client.fd, err);
								return (-1);
							}

							channel.operators.insert(targetFd);
							std::string mode_msg = ":" + client.nickname + "!" + client.username + "@" + 
								client.hostname + " MODE " + channel.name + " " + sign + "o " + targetNick + "\r\n"; 	//! SEND TO every member on the channel
							sendToAllChannelMembers(channel, mode_msg);
						}
						else
						{
							if (client.fd == targetFd && channel.operators.size() == 1)
							{
								std::string err = ERR_NOPRIVILEGES(std::string(SERV), client.nickname);
								ft_send(client.fd, err);
								return (-1);
							}

							if (channel.operators.count(targetFd) == 0)
							{
								std::string err = ERR_CANNOTREMOVEOPER(std::string(SERV), client.nickname, channel.name, targetNick);
    							ft_send(client.fd, err);
								return (-1);
							}
							
							channel.operators.erase(targetFd);
							std::string mode_msg = ":" + client.nickname + "!" + client.username + "@" + 
								client.hostname + " MODE " + channel.name + " " + sign + "o " + targetNick + "\r\n"; 	//! SEND TO every member on the channel
							sendToAllChannelMembers(channel, mode_msg);
						}
						break;
					}
				
				default:

					std::string err = ERR_UNKNOWNMODE(std::string(SERV), client.nickname, std::string(1, c));
					if (ft_send(client.fd, err))
						return (-1);
					break;

				}

			}
	}

	//# at the end Notify channel what changes have been made
	//? am i already doing this with the sendToALLChannelMembers function?
	return (0);
}
