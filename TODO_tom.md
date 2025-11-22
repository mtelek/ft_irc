# tom TODOs

- after closing the server still blocks the server_port
- possible solutions;

	THIS IS NOT WORKING FOR SOME REASON -> fix this

	int opt = 1;
	setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	WRITE A SIGHANDLER: WHEN PROGRAMM RECIEVES SIGINT IT UNBINDS THE SOCKET

	ERROR ONLY HAPPENS WHEN U RUN ./irc_exe & to return the pid;


