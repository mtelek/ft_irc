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


	std::map<std::string, Channel>::iterator it = channels_.find(name);
	if (it == channels_.end())
	{
		std::string err = ERR_NOSUCHCHANNEL(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (-1);
	}
	Channel& channel = it->second;

	if (channel.members.count(client.fd) == 0)
	{
		std::cout << "Error: you're not part of the channel" << std::endl;		//! SEND TO CLIENT
		return (-1);
	}

	std::string token;
	iss >> token;
	if (token.empty())
	{
		std::cout << getModes(channel) << std::endl;			//! SEND TO CLIENT
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
							std::cout << "Error: +k needs a key" << std::endl;   	//! SEND TO CLIENT
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
							std::cout << "Error: +l needs a limit" << std::endl;   	//! SEND TO CLIENT
                        	return (-1);
						}

						if (!isNumber(trim(params[paramsUsed])))
						{
							std::cout << "Error: limit can only contain numbers" << std::endl;   	//! SEND TO CLIENT
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
							std::cout << "Error: +o/-o needs a username" << std::endl;   	//! SEND TO CLIENT
                        	return (-1);
						}

						if (channel.operators.count(client.fd) == 0) 
						{
    						std::cout << "Error: You're not a channel operator" << std::endl;		//! SEND TO CLIENT
    						return (-1);
						}

						std::string targetNick = params[paramsUsed];
						paramsUsed++;

						int targetFd = findClientByNick(targetNick);
						if (targetFd == -1)
						{
							std::cout << "Error: no such nickname" << targetNick << std::endl;		//! SEND TO CLIENT
    						return (-1);
						}

						if (channel.members.count(targetFd) == 0)
						{
							std::cout << "Error: user not found in channel" << targetNick << std::endl;		//! SEND TO CLIENT
    						return (-1);
						}

						if (sign == '+')
						{
							if (client.fd == targetFd)
							{
								std::cout << "Error: you are already operator" << std::endl;		//! SEND TO CLIENT
    							return (-1);
							}

							if (channel.operators.count(targetFd))
							{
								std::cout << targetNick << " is already operator" << std::endl;		//! SEND TO CLIENT 
								return (-1);														//maybe change to non fatal
							}

							channel.operators.insert(targetFd);
							std::cout << targetNick << " is now operator in " << channel.name <<std::endl;		//! SEND TO CLIENT
						}
						else
						{
							if (client.fd == targetFd && channel.operators.size() == 1)
							{
								std::cout << "Error: you cant rm operator rights bc you are only operator" << std::endl;		//! SEND TO CLIENT
    							return (-1);
							}

							if (channel.operators.count(targetFd) == 0)
							{
								std::cout << "Error: cant rm operator right from " << targetNick << "bc user is not operator" << std::endl;		//! SEND TO CLIENT
								return (-1);
							}
							
							channel.operators.erase(targetFd);
							std::cout << targetNick << " is no longer operator in " << channel.name <<std::endl;		//! SEND TO CLIENT
						}
						break;
					}
				
				default:

					std::cout << "Warning: unknown mode '" << c << "' ignored" << std::endl; //!SEND to CLIENT
					return (-1);
                    break;

				}

			}
	}

	//# at the end Notify channel what changes have been made
    return (0);
}
