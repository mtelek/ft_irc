/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   username.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:21:08 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 18:49:11 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::setUser(Client &client, std::istringstream &iss)
{
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);
	realname = trim(realname);
	
	std::cout << formatDate() << "Client#" << client.fd << " -> USER " << username << " " << hostname << " " << servername << " " << realname << "\n";
	if (client.isRegistered == true)
	{
		//INVALID - REREGISTRATION
		std::string error = E462(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		//INVALID - EMPTY ARGUMENTS
		std::string error = E461(std::string(SERV), client.nickname, "USER");
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	if (!isValidLength(username, 9) || !isValidLength(realname, 9))
	{
		//USERNAME OR REALNAME TOO LONG
		std::string error = E4323(std::string(SERV), client.nickname);
		send(client.fd, error.c_str(), error.length(), 0);
		return ;
	}
	else if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);
	client.username = username;
	client.realname = realname;
	client.hasUser = true;
	
	//USERNAME SET
	std::string success = S411(std::string(SERV), client.nickname, username);
	send(client.fd, success.c_str(), success.length(), 0);
	
	//REALNAME SET
	success = S412(std::string(SERV), client.nickname, realname);
	send(client.fd, success.c_str(), success.length(), 0);
}
