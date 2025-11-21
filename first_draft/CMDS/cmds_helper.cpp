#include "../server.hpp"

std::string		server::getModes(Channel &channel)
{
	std::ostringstream ss;
	std::string flags = "+";

	if (channel.isInviteOnly)  flags += "i";
	if (channel.isTopicLocked) flags += "t";
	if (!channel.key.empty())  flags += "k";
	if (channel.userLimit > 0) flags += "l";

	ss << flags;
	if (!channel.key.empty())
		ss << " " << channel.key;
	if (channel.userLimit > 0)
		ss << " " << channel.userLimit;

	return ss.str();	
}

bool	server::isNumber(const std::string& s)
{
    if (s.empty())
        return (false);

    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        if (!std::isdigit(static_cast<unsigned char>(*it)))
            return (false);
    }
    return true;

}

int		server::findClientByNick(const std::string& nick)
{
	for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); it++)
	{
		if (it->second.nickname == nick)
			return (it->second.fd);
	}
	return (-1);
}
