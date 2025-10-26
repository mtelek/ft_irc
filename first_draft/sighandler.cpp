/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:47:49 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 18:22:32 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::sigHandler(int signal)
{
    (void)signal;
    std::cout << "\n" << formatDate() << "Received shutdown signal. Cleaning up..." << std::endl;
    running_ = 0;
}

void server::cleanUp()
{    
    // CLOSE CLIENTS
    for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        std::cout << formatDate() << "Closing Client#" << it->first << std::endl;
        close(it->first);
    }
    clients_.clear();
    // CLOSE SERVER
    if (server_fd_ != -1)
    {
        std::cout << formatDate() << "Closing server socket" << std::endl;
        close(server_fd_);
        server_fd_ = -1;
    }
    std::cout << formatDate() << "Server shutdown complete." << std::endl;
}
