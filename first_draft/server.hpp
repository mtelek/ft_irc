#ifndef SERVER_HPP
#define SERVER_HPP

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

class server 
{
    private:

        int             server_fd_;
        sockaddr_in     server_addr_;    
        int             port_;
        std::string     password_;
    
    public:
        server();
        ~server();

    //# INITIALIZE SERVER
    //# 1. CREATING SOCKET
    //# 2. SETTING UP THE ADDRESS
    //# 3. BINDING THE SOCKET TO ADDRESS

    //# GOAL: after calling init all the the server variables are intialized
    //# and the server socket is bound to given port (server does not listen() yet)!

    int     init(int port, std::string password);

    //# RUN SERVER
    //# 1. LISTENING FOR CONNECTIONS
    //# 2. ACCEPTING CONNECTIONS

    void    run();
};

#endif
