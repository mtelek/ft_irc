#include "server.hpp"

//# USAGE: run ./irc password port and in another therminal run "nc localhost 6667"

bool isValidPort(const std::string& portStr, int& port)
{
	if (portStr.empty())
		return (false);
	
	for (std::string::size_type i = 0; i < portStr.length(); ++i)
	{
		if (!std::isdigit(portStr[i]))
			return (false);
	}
	port = std::atoi(portStr.c_str());
	return (port >= 1024 && port <= 65535);
}

bool isValidPassword(const std::string& password)
{
	if (password.empty())
		return (false);
	
	for (std::string::size_type i = 0; i < password.length(); ++i)
	{
		if (password[i] < 32 || password[i] > 126)
			return (false);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc != 3 || argc >= 4)
		return (std::cerr << "Usage: ./irc <port> <password>\n", 1);
	int port;
	if (!isValidPort(argv[1], port))
		return (std::cerr << "Error: Port must contain only digits and be between 1024 and 65535\n", 1);
	
	std::string password = argv[2];
	if (!isValidPassword(password))
		return (std::cerr << "Error: Password must contain only printable ASCII characters\n", 1);

	server server;
	std::cout << server.formatDate(2) << "Server PID: " << getpid() << std::endl;
	if (server.init(port, password) == -1)
		return (1);

	server.run();
}
