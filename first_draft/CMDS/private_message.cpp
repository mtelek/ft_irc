
#include "../server.hpp"

int server::sendToAllChannelMembers(Channel &channel, std::string &message)  //? new fucntion
{
	std::set<int>::iterator it;
	for (it = channel.members.begin(); it != channel.members.end(); ++it)
	{
		ft_send(*it, message);
		//! close client here as well
	}
	return (0);
}

int		server::isChannel(std::string &target)
{
	if (channels_.count(target) == 0)
		return (0);
	return (1);
}

int		server::privmsg2Channel(Client &client, std::string &target, std::string &message)
{
	std::map<std::string, Channel>::iterator it = channels_.find(toLowerString(target));

	if (it == channels_.end())
	{
		std::string err = ERR_NOSUCHCHANNEL(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}
	Channel& channel = it->second;

	if (channel.members.count(client.fd) == 0)
	{
		std::string err = ERR_NOTONCHANNEL(std::string(SERV), client.nickname, channel.name);
		ft_send(client.fd, err);
		return (1);
	}

	if (message.empty())
	{
		std::string err = ERR_NOTEXTTOSEND(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}

	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);
	
	std::string	out_msg = ":" + client.nickname +  " PRIVMSG " + target + " :" + message + "\r\n";
	for  (std::set<int>::iterator m = channel.members.begin(); m != channel.members.end(); m++)
	{
		int member_fd = *m;
		
		if (member_fd == client.fd)				continue;
		if (clients_.count(member_fd) == 0)		continue;

		if (ft_send(member_fd, out_msg) == -1)
			return (-1);
	}
	return (0);
}

int		server::privmsg2Client(Client &client, std::string &target, std::string &message)
{
	int target_fd = findClientByNick(target);
	if (target_fd == -1)
	{
		std::string err = E401(std::string(SERV), client.nickname, target);
		ft_send(client.fd, err);
		return (-1);
	}
	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);
	
	std::string new_message = ":" + client.nickname + " PRIVMSG " + target + " :" + message + "\r\n";
	ft_send(target_fd, new_message);
	return (0); 
}

int		server::ft_send(int fd, std::string &message)	//! maybe change this funcion (MSG_NOSIGNAL, broken pipe)
{
	int bytessend = 0;
	int	bytesleft = message.length();
	int ret;

	while (bytessend < (int)message.length())
	{
		ret = send(fd, message.c_str() + bytessend, bytesleft, MSG_DONTWAIT);
		if (ret == -1)	break;
		bytessend += ret;
		bytesleft -= ret;
	}

	if (ret == -1)
	{
		//! delete the client and deletes all the data and clears everything, also closing the fds
		return (-1);
	}
	return (0);
}

void server::sendToAllClients(Client &sender, const std::string &message)
{
	std::string privmsg = ":" + sender.nickname + " PRIVMSG $" + " :" + message + "\r\n";
	
	std::map<int, Client>::iterator it;
	for (it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (it->first != sender.fd && it->second.isRegistered)
			ft_send(it->first, privmsg);
	}
}

int server::sendPrivate(Client &client, std::istringstream &iss)
{
	std::string target;
	iss >> target;

	std::string message;
	std::getline(iss, message);
	message = trim(message);
	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);   

	if (!client.isRegistered) 
	{
		std::string err =  ERR_NOTREGISTERED(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}
	if (target.find(',') != std::string::npos)
	{
		std::string err = ERR_TOOMANYCHAN(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}
	if (target.empty())
	{
		std::string err = ERR_NORECIPIENT(std::string(SERV), client.nickname, "PRIVMSG");
		ft_send(client.fd, err);
		return (1);
	}
	if (message.empty()) 
	{
		std::string err = ERR_NOTEXTTOSEND(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}
	if (!isChannel(target) && target[0] == '#')
	{
		std::string err = ERR_NOSUCHCHANNEL(std::string(SERV), client.nickname);
		ft_send(client.fd, err);
		return (1);
	}
	if (isChannel(target))
		privmsg2Channel(client, target, message);
	else if (target[0] == '$')
		sendToAllClients(client, message);
	else
		privmsg2Client(client, target, message);
	return (0);
}

