#include "server.hpp"

server::server() : server_fd_(-1), port_(0)
{
   std::memset(&server_addr_, 0, sizeof(server_addr_));
}

server::~server() {}

int     server::init(int port, std::string password)
{

	port_ = port;
	password_ = password;

	//# CREATE A TCP SOCKET

	server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd_ == -1)
	{
		std::cerr << "Error: cannot create socket" << std::endl;
		return (-1);
	}

	//# ALLOW QUICK REUSE OF THE SAME PORT AFTER SERVER RESTART

	int opt = 1;
	setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//# SET UP THE SERVER ADDRESS (LISTEN IN ALL INTERFACES ON CHOSEN PORT)

	std::memset(&server_addr_, 0, sizeof(server_addr_));
	server_addr_.sin_family = AF_INET;
	server_addr_.sin_port = htons(port_);
	server_addr_.sin_addr.s_addr = INADDR_ANY;

	//# BIND SOCKET TO THE CONFIGURED IP AND PORT
	// configured ip = INADDR_ANY = all available network interfaces on this computer
	// port = given port by user

	if (bind(server_fd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) == -1) 
	{
		std::cerr << "Error: cannot bind socket" << std::endl;
		close(server_fd_);
		return (-1);
	}

	std::cout << "Socket successfully created and bound on port " << port << std::endl;
	return (0);
}

void server::initClient(int client_fd, sockaddr_in client_addr)
{
	Client& client = clients_[client_fd];
	client.fd = client_fd;
	client.hasNick = false;
	client.hasUser = false;
	client.isRegistered = false;
	client.hasPassword = false;
	client.buffer = "";
	client.wrongPass = 0;

	client.hostname = inet_ntoa(client_addr.sin_addr);
	client.port = ntohs(client_addr.sin_port);
	std::cout << "Client connected from: " << client.hostname << ":" << client.port << std::endl;
}

void    server::run()
{   
	//# START LISTENING (Allow up to 128 pending connections that haven’t been accepted yet)

	if(listen(server_fd_, 128) == -1)
	{
		std::cerr << "Error: cannot listen on socket" << std::endl;
		close(server_fd_);
		return ;
	}

	std::cout << "Server listening in port " << port_ << std::endl;

	//# MAKE SERVER SOCKET NON-BLOCKING (Instantly returns instead of holding up programm)
	fcntl(server_fd_, F_SETFL, O_NONBLOCK);

	//# SET UP POLL STRUCTURE

	// struct pollfd    (existing struct from poll.h)
	// {
	//     int   fd;        // file descriptor (socket)
	//     short events;    // what you want to watch for
	//     short revents;   // what actually happened (set by poll())
	// };

	std::vector<pollfd> fds;
	pollfd server_poll;
	server_poll.fd = server_fd_;
	server_poll.events = POLLIN;
	server_poll.revents = 0;
	fds.push_back(server_poll);

	//# MAIN SERVER LOOP
	while (true)
	{
		int x = poll(&fds[0], fds.size(), -1);
		if (x < 0)
		{
			std::cerr << "Error: poll() failed" << std::endl;
			break;
		}

		if (fds[0].revents & POLLIN)
		{
			sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);
			int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &client_len);
			if (client_fd != -1)
			{
				fcntl(client_fd, F_SETFL, O_NONBLOCK);

				pollfd client_poll;
				client_poll.fd = client_fd;
				client_poll.events = POLLIN;
				client_poll.revents = 0;
				fds.push_back(client_poll);
				initClient(client_fd, client_addr);
			}
		}

		for (size_t i = 1; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				char buffer[1024];
				ssize_t bytes = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (bytes <= 0)
				{
					std::cout << "Client disconnected: FD = " << fds[i].fd << std::endl;
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					i--;
				}
				else
					if (recieveMessage(fds, i, buffer, bytes) == -1)
					{
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--;
					}
			}
		}
	}
	close(server_fd_);
}

