#include "server.hpp"

server::server() : server_fd_(-1), port_(0)
{
   std::memset(&server_addr_, 0, sizeof(server_addr_));
}


server::~server() 
{
}

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
