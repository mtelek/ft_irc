#include "helper.hpp"
#include "server.hpp"


//# USAGE: run ./irc password port and in another therminal run "nc localhost 6667"

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

	server.run();
}
