/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_helper.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:23:54 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 18:37:26 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool server::isValidLength(const std::string& str, size_t maxLength)
{
	return (str.length() <= maxLength);
}

std::string server::trim(const std::string& str)
{
	size_t start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
		return ("");
	size_t end = str.find_last_not_of(" \t\n\r\f\v");
	return (str.substr(start, end - start + 1));
}

std::string server::formatDate()
{
	std::string date = "[" + getStartDate() + "] ";
	return (date);
}

std::string server::toLowerString(const std::string& str)
{
	std::string lower = str;
	for (std::string::iterator it = lower.begin(); it != lower.end(); ++it)
		*it = std::tolower(*it);
	return (lower);
}

std::string	server::getStartDate()
{
	//GETTING THE START DATE OF THE SERVER OR THE SERVER MESSAGE
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);
	
	std::stringstream ss;
	ss << std::setfill('0');
	ss << std::setw(2) << localTime->tm_hour << ":"
		<< std::setw(2) << localTime->tm_min << ":"
		<< std::setw(2) << localTime->tm_sec << " "
		<< std::setw(2) << localTime->tm_mday << "/"
		<< std::setw(2) << (localTime->tm_mon + 1) << "/"
		<< (localTime->tm_year + 1900);
	
	return (ss.str());
}
