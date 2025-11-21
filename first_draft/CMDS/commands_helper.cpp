
#include "../server.hpp"

bool server::isValidLength(const std::string& str, size_t maxLength)
{
	return (str.length() <= maxLength);
}

std::string server::trim(const std::string& str)
{
	size_t start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
		return ("");
	size_t end = str.find_last_not_of(" \t\n\r\f\v");
	return (str.substr(start, end - start + 1));
}

std::string server::formatDate()
{
	std::string date = "[" + getStartDate() + "] ";
	return (date);
}

std::string server::toLowerString(const std::string& str)
{
	std::string lower = str;
	for (std::string::iterator it = lower.begin(); it != lower.end(); ++it)
		*it = std::tolower(*it);
	return (lower);
}

std::string	server::getStartDate()
{
	//GETTING THE START DATE OF THE SERVER OR THE SERVER MESSAGE
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);
	
	std::stringstream ss;
	ss << std::setfill('0');
	ss << std::setw(2) << localTime->tm_hour << ":"
		<< std::setw(2) << localTime->tm_min << ":"
		<< std::setw(2) << localTime->tm_sec << " "
		<< std::setw(2) << localTime->tm_mday << "/"
		<< std::setw(2) << (localTime->tm_mon + 1) << "/"
		<< (localTime->tm_year + 1900);
	
	return (ss.str());
}

void	server::sendWelcome(Client &client)
{
	std::string welcome = RPL_WELCOME(std::string(SERV), client.nickname, SERVER_NAME, client.username, client.hostname);
	std::string yourhost = RPL_YOURHOST(std::string(SERV), client.nickname, SERVER_NAME, VERSION);
	std::string created = RPL_CREATED(std::string(SERV), client.nickname, startDate);
	std::string myinfo = RPL_MYINFO(std::string(SERV), client.nickname, SERVER_NAME, VERSION);
	std::string isupport = RPL_ISUPPORT(std::string(SERV), client.nickname);
	send(client.fd, welcome.c_str(), welcome.length(), MSG_DONTWAIT);
	send(client.fd, yourhost.c_str(), yourhost.length(), MSG_DONTWAIT);
	send(client.fd, created.c_str(), created.length(), MSG_DONTWAIT);
	send(client.fd, myinfo.c_str(), myinfo.length(), MSG_DONTWAIT);
	send(client.fd, isupport.c_str(), isupport.length(), MSG_DONTWAIT);
}

void	server::checkRegistration(Client &client)
{
	if (client.hasPassword && client.hasNick && client.hasUser && !client.isRegistered)
	{
		client.isRegistered = true;
		std::cout << formatDate() << "Client#" << client.fd << "'s (" << client.nickname << ") registration successful\n";
		sendWelcome(client);
	}
}
