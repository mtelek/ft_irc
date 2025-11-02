
#include "../server.hpp"

bool server::isNameTaken(std::string Client::* member, const std::string& name)
{
	//CHECKING ALREADY TAKEN NAMES
	std::string lowerName = toLowerString(name);
	for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (toLowerString(it->second.*member) == lowerName)
			return (true);
	}
	return (false);
}

bool server::isValidName(Client &client, std::string &nickname)
{
	if (nickname.empty())
	{
		//NO NICKNAME
		std::string error = E431(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		return (false);
	}
	const std::string forbiddenChars = " ,*?!@";
	for (size_t i = 0; i < nickname.length(); ++i)
	{
		//FORBIDDEN CHARACTERS
		if (forbiddenChars.find(nickname[i]) != std::string::npos)
		{
			std::string error = E432(std::string(SERV), client.nickname);
			send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
			return (false);
		}
	}
	if (nickname[0] == '$' || nickname[0] == ':' || !isValidLength(nickname, 9))
	{
		//FORBIDDEN CHARACTERS
		std::string error = E432(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		return (false);
	}
	const std::string Prefixes = "#&@+"; // or just only &
	if (Prefixes.find(nickname[0]) != std::string::npos)
	{
		//FORBIDDEN PREFIXES
		std::string error = E432(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		return (false);
	}
	else if (nickname.find('.') != std::string::npos)
	{
		//WARNING - DOT SHOULDNT BE USED
		std::string warning = E4322(std::string(SERV), client.nickname);
		send(client.fd, warning.c_str(), warning.length(), MSG_DONTWAIT);
		return (true);
	}
	return (true);
}

void server::setNick(Client &client, std::istringstream &iss)
{
	std::string nickname;
	iss >> nickname;

	std::cout << formatDate() << "Client#" << client.fd << " -> NICK " << nickname << std::endl;
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::string error = E462(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		return ;
	}
	//CAN SET NICKNAME - PASSWORD VALID
	if (isNameTaken(&Client::nickname, nickname) == true)
	{
		//NICKNAME TAKEN
		std::string error = E433(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
	}
	else if (isValidName(client, nickname) == false)
		return ;
	else
	{
			//VALID NICKNAME
			client.nickname = nickname;
			client.hasNick = true;
			std::string success = S435(std::string(SERV), client.nickname);
			send(client.fd, success.c_str(), success.length(), MSG_DONTWAIT);
	}
}
