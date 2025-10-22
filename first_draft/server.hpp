#ifndef SERVER_HPP
#define SERVER_HPP

#define SERVER_NAME "localhost"
#define VERSION "1.0"
#define MAX_PASS_ATTEMPTS 3

#include <iostream>
#include <cstring>      // for memset
#include <cstdlib>      // for exit()
#include <unistd.h>     // for close()
#include <arpa/inet.h>  // for sockaddr_in, inet_ntoa()
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <map>
#include <sstream>
#include "error_messages.hpp"
#include <ctime>         // for time and localtime
#include <iomanip>		// for setw and setfill

class server 
{
	private:

		int             server_fd_;
		sockaddr_in     server_addr_;    
		int             port_;
		std::string     password_;
		std::string		startDate;

		struct Client 
		{
			// Set by server during connection
			int fd;
			int port;
			std::string hostname; //ip address

			// Set by client during registration
			std::string nickname;
			std::string username;
			std::string realname;

			// Registration state tracking
			bool hasNick;
        	bool hasUser;
			bool hasPassword;
			bool isRegistered;

			std::string buffer;
			int wrongPass;
		};

		std::map <int, Client> clients_;               // fd -> client
		// std::map <std::string, int> nickname2fd        // nickname -> fd
		
		// struct  Room
		// {
				// std::map <int, Client> clients_room_;
				// bool isinvte;
		// }

		//INIT
		void	initClient(int client_fd, sockaddr_in client_addr);
		int		executeCommands(int client_fd, const std::string& command);
		int		recieveMessage(std::vector<pollfd> fds, size_t i, char *buffer, ssize_t bytes);

		//COMMANDS
		int		authenticate(Client &client, std::istringstream &iss);
		void	setNick(Client &client, std::istringstream &iss);
		void	setUser(Client &client, std::istringstream &iss);
		int		quit(Client &client, std::istringstream &iss);

		//HELPER
		bool	maxAttemptsReached(Client &client);
		bool	isNameTaken(std::string Client::* member, const std::string& name);
		void	checkRegistration(Client &client);
		void	sendWelcome(Client &client);
		std::string	getStartDate();

		//SENDING MESSAGE COMMANDS
		void		sendPrivate(Client &client, std::istringstream &iss);
		std::string	trim(const std::string& str);

	public:
		server();
		~server();

	int     init(int port, std::string password);
	void    run();
};

#endif
