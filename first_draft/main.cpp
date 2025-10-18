#include "helper.hpp"
#include "server.hpp"


int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	// if (argc != 3)
	// {
	// 	std::cerr << "Usage: ./irc <port> <password>" << std::endl;
	// 	return (1);
	// }

	// int	port = atoi(argv[1]);
	// std::string password = argv[2];

	int port = 6667;
	std::string password = "i<3feet";

	server server;
	if (server.init(port, password) == -1)
		return (1);

	//server.run();
}


// int main(int argv, char **argc)
// {
// 	(void)argc;
// 	(void)argv;

// 	int server_fd;
// 	struct sockaddr_in server_addr;

// 	//# CREATING SOCKET (IPv4, TCP)

// 	server_fd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (server_fd == -1)
// 	{
// 		std::cerr << "Error: cannot create socket" << std::endl;
// 		return (1);
// 	}

// 	//# SETTING UP THE ADDRESS

// 	std::memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(6667);           // Port 6667
//     server_addr.sin_addr.s_addr = INADDR_ANY;     // Accept connections from any IP


// 	//# BIND SOCKET TO ADDRESS

// 	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
// 	{
//         std::cerr << "Error: cannot bind socket" << std::endl;
//         close(server_fd);
//         return (1);
//     }

// 	//# START LISTENING FOR CONNECTIONS

//     if (listen(server_fd, 5) == -1) 
// 	{
//         std::cerr << "Error: cannot listen on socket" << std::endl;
//         close(server_fd);
//         return 1;
//     }

//     std::cout << "Server listening on port 6667..." << std::endl;

//     while (true) {
//         sockaddr_in client_addr;
//         socklen_t client_len = sizeof(client_addr);

//         int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
//         if (client_fd == -1) {
//             std::cerr << "Error: accept failed\n";
//             continue;
//         }

//         std::cout << "✅ Client connected from "
//                   << inet_ntoa(client_addr.sin_addr)
//                   << ":" << ntohs(client_addr.sin_port) << std::endl;

//         close(client_fd);   // for now we just close immediately
//     }

//     close(server_fd);
//     return 0;
// }