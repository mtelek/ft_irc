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
#include <map>

class server 
{
    private:

        int             server_fd_;
        sockaddr_in     server_addr_;    
        int             port_;
        std::string     password_;

        // struct Client 
        // {
        //     int fd;
        //     int port;
        //     std::string hostname; //ip
        //     std::string nickname;
        //     std::string realname;
        //     std::string password;
        //     std::string buffer;
        //     bool isRegisterd;
        //     bool hasPassword;
        //     bool hasDisconected;
        // }

        // std::map <int, Client> clients_;               // fd -> client
        // std::map <std::string, int> nickname2fd        // nickname -> fd
        
        // struct  Room
        // {
                // std::map <int, Client> clients_room_;
                // bool isinvte;
        // }

    
    public:
        server();
        ~server();

    int     init(int port, std::string password);
    void    run();
};

#endif
