#ifndef SERVER_HPP
#define SERVER_HPP

#define SERVER_NAME "localhost"
#define VERSION "1.0"
#define MAX_PASS_ATTEMPTS 3
#define SERV ":server.1"

#include "error_messages.hpp"
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
#include <ctime>         // for time and localtime
#include <iomanip>		 // for setw and setfill
#include <signal.h>      // for sighandler
#include <set>
#include <csignal>      // For signal handling
#include <cerrno>

class server 
{
	private:

		int             server_fd_;
		sockaddr_in     server_addr_;    
		int             port_;
		std::string     password_;
		std::string		startDate;

		//SIGHANDLER
		static volatile sig_atomic_t running_;

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

			std::set<std::string> joinedChannels;
		};

		std::map <int, Client> clients_;               // fd -> client
		// std::map <std::string, int> nickname2fd        // nickname -> fd
		
		struct  Channel
		{
				std::string name;		// hast to start with '#'
				std::string topic;		// optional;

				std::set<int>			members;
				std::set<int>			operators;
				std::set<std::string>	invited;
				
				bool			isInviteOnly;	// +i
				bool			isTopicLocked;	// +t
				int				userLimit;		// +l (0 = no limit)
				std::string		key;		// +k
		};

		std::map <std::string, Channel> channels_;

		//INIT
		void	initClient(int client_fd, sockaddr_in client_addr);
		int		executeCommands(int client_fd, const std::string& command);
		int		recieveMessage(std::vector<pollfd> fds, size_t i, char *buffer, ssize_t bytes);

		//COMMANDS
		int		authenticate(Client &client, std::istringstream &iss);		//# PASS
		void	setNick(Client &client, std::istringstream &iss);			//# NICK
		void	setUser(Client &client, std::istringstream &iss);			//# USER
		int		quit(Client &client, std::istringstream &iss);				//# QUIT
		int		join(Client &client, std::istringstream &iss);				//# JOIN
		// int		part(Client &client, std::istringstram &iss);				//# PART
		// int		topic(Client &client, std::istringstram &iss);				//# TOPIC
		// int		mode(Client &client, std::istringstram &iss);				//# MODE
		// int		kick(Client &client, std::istringstram &iss);				//# KICK
		// int		invite(Client &client, std::istringstram &iss);				//# INVITE

		//CHANNEL HELPER
		int		initChannel(int fd, std::string& name);
		// int		addUser(int fd, Channel& channel);

		//USER HELPER
		bool	maxAttemptsReached(Client &client);
		bool	isNameTaken(std::string Client::* member, const std::string& name);
		void	checkRegistration(Client &client);
		void	sendWelcome(Client &client);
		bool	isValidName(Client &client, std::string &nickname);
		static std::string	getStartDate();
		std::string toLowerString(const std::string& str);

		//SENDING MESSAGE COMMANDS
		void		sendPrivate(Client &client, std::istringstream &iss);
		std::string	trim(const std::string& str);

		//SIGHANDLER
		static void	sigHandler(int signal);
		void	cleanUp();

	public:
		server();
		~server();

	int     init(int port, std::string password);
	void    run();
	static std::string formatDate();
};

#endif
