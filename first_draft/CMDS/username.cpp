
#include "../server.hpp"

int server::setUser(Client &client, std::istringstream &iss)
{
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);
	realname = trim(realname);
	
	std::cout << formatDate() << "Client#" << client.fd << " -> USER " << username << " " << hostname << " " << servername << " " << realname << "\n";
	if (!client.hasPassword) //! you sure about return value?
		return (0);
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::string error = E462(std::string(SERV), client.nickname);
		ft_send(client.fd, error);
		return (0);
	}
	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		//INVALID - EMPTY ARGUMENTS
		std::string error = E461(std::string(SERV), client.nickname, "USER");
		ft_send(client.fd, error);
		return (0);
	}
	if (!isValidLength(username, 9) || !isValidLength(realname, 9))
	{
		//USERNAME OR REALNAME TOO LONG
		std::string error = E4323(std::string(SERV), client.nickname);
		ft_send(client.fd, error);
		return (0);
	}
	else if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);
	client.username = username;
	client.realname = realname;
	client.hasUser = true;
	
	//USERNAME SET
	std::string success = S411(std::string(SERV), client.nickname, username);
	ft_send(client.fd, success);
	
	//REALNAME SET
	success = S412(std::string(SERV), client.nickname, realname);
	ft_send(client.fd, success);
	return (0);
}
