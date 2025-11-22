
#include "../server.hpp"

void server::setUser(Client &client, std::istringstream &iss)
{
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);
	realname = trim(realname);
	
	std::cout << formatDate() << "Client#" << client.fd << " -> USER " << username << " " << hostname << " " << servername << " " << realname << "\n";
	if (!client.hasPassword) //!  still wrong, after trying to register twice
		return ;
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::string error = E462(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		return ;
	}
	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		//INVALID - EMPTY ARGUMENTS
		std::string error = E461(std::string(SERV), client.nickname, "USER");
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		return ;
	}
	if (!isValidLength(username, 9) || !isValidLength(realname, 9))
	{
		//USERNAME OR REALNAME TOO LONG
		std::string error = E4323(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), MSG_DONTWAIT);
		return ;
	}
	else if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);
	client.username = username;
	client.realname = realname;
	client.hasUser = true;
	
	//USERNAME SET
	std::string success = S411(std::string(SERV), client.nickname, username);
	send(client.fd, success.c_str(), success.length(), MSG_DONTWAIT);
	
	//REALNAME SET
	success = S412(std::string(SERV), client.nickname, realname);
	send(client.fd, success.c_str(), success.length(), MSG_DONTWAIT);
}
