/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:48:29 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/26 01:42:57 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//AUTHENTICATE PASSWORD
#define E464(serv,nick)  serv + " 464 " + nick + " :Password incorrect\r\n"
#define E4642(serv,nick) serv + " 4642 " + nick + " :Password incorrect. Disconnecting\r\n"
#define S464(serv,nick) serv + " 464 " + nick + " :Authentication successful\r\n"
#define E462(serv,nick) serv + " 462 " + nick + " :You may not reregister\r\n"

//SETNICK
#define E433(serv,nick) serv + " 433 " + nick + " :Nickname is already in use\r\n"
#define E431(serv,nick) serv + " 431 " + nick + " :No nickname given\r\n"
#define S433(serv, nick) serv + " 433 " + nick + " :Nickname set to " + nick + " \r\n"
#define E4643(serv,nick) serv + " 4643 " + nick + " :Authentication required\r\n"
#define E432(serv,nick) serv + " 432 " + nick + " :Erroneous nickname\r\n"
#define E4322(serv,nick) serv + " 4322 " + nick + " :Warning: Erroneous nickname\r\n"
#define  E431(serv,nick) serv + " 431 " + nick + " :No nickname given\r\n"

//QUIT
#define S410(serv,nick) serv + " 410 " + nick +" :Disconnected\r\n"

//SETUSER
#define S411(serv, nick, user) serv + " 411 " + nick + " :Username set to " + user + "\r\n"
#define S412(serv, nick, real) serv + " 412 " + nick + " :Realname set to " + real + "\r\n"

//EXECUTE COMMANDDS
#define E461(serv, nick, cmd) serv + " 461 " + nick + " :" + cmd + " :Not enough parameters\r\n"
#define E463(serv, nick, cmd) serv + " 463 " + nick + " :" + cmd + " :Invalid command\r\n"

//SENDPRIVATE
#define S465(serv,nick) serv + " 465 " + nick + " :Message sent\r\n"
#define E412(serv,nick) serv + " 412 " + nick + " :No text to send\r\n"
#define E401(serv, nick, target) serv + " 401 " + nick + " :" + target + " :No such nick/channel\r\n"

//CLIENT WELCOME MESSAGE
#define RPL_WELCOME(serv, nick, server, user, host) serv + " :Welcome to the " + server + " Network, " + nick + "[!" + user + "@" + host + "]\r\n"
#define RPL_YOURHOST(serv, server, version) serv + " :Your host is " + server + " running version " + version + "\r\n"
#define RPL_CREATED(serv, date) serv + " :This server was created " + date + "\r\n"
#define RPL_MYINFO(serv, server, version) serv + " :" + server + " " + version + " 0 0\r\n"  //might wanna change it after impleemnted channels
#define RPL_ISUPPORT(serv) serv + " :CASEMAPPING=rfc1459 CHANTYPES=# CHANMODES=itl PREFIX=(ov)@+ CHANLIMIT=#: MAXNICKLEN=9 NICKLEN=9 TOPICLEN=307 KICKLEN=307 CHANNELLEN=200 USERLEN=9\r\n"
//RPL_ISUPPORT isnt correct fully yet

// // 400-499: Client errors
// #define E403 "<channel name> :No such channel"
// #define E404 "<channel name> :Cannot send to channel"
// #define E405 "<channel name> :You have joined too many channels"
// #define E406 "<nickname> :There was no such nickname"
// #define E407 "<target> :Duplicate recipients. No message delivered"
// #define E409 ":No origin specified"
// #define E451 ":You have not registered"

// // Registration errors
// #define E431 ":No nickname given"
// #define E432 "<nick> :Erroneous nickname"
// #define E436 "<nick> :Nickname collision KILL"

// // Authentication errors  
// #define E462 ":You may not reregister"
// #define E465 ":You are banned from this server"

// // Channel errors
// #define E442 "<channel> :You're not on that channel"
// #define E443 "<user> <channel> :is already on channel"
// #define E451 ":You have not registered"
// #define E471 "<channel> :Cannot join channel (+l)"
// #define E473 "<channel> :Cannot join channel (+i)"
// #define E474 "<channel> :Cannot join channel (+b)"
// #define E475 "<channel> :Cannot join channel (+k)"
// #define E476 "<channel> :Bad Channel Mask"


// // MOTD (Message of the Day)
// #define E375 ":- <server> Message of the Day - "
// #define E372 ":- <text>"
// #define E376 ":End of /MOTD command"

// // Channel responses
// #define E353 "<channel> :[[@|+]<nick> [[@|+]<nick> ...]]"
// #define E366 "<channel> :End of /NAMES list"
// #define E332 "<channel> :<topic>"
// #define E333 "<channel> <nick> <time>"

// // Who/User information
// #define E352 "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"
// #define E315 "<name> :End of /WHO list"
// #define E311 "<nick> <user> <host> * :<real name>"
// #define E312 "<nick> <server> :<server info>"
// #define E318 "<nick> :End of /WHOIS list"

// // PRIVMSG/NOTICE errors
// #define E411 ":No recipient given (<command>)"

// // MODE errors
// #define E472 "<char> :is unknown mode char to me"
// #define E502 ":Cant change mode for other users"
// #define E501 ":Unknown MODE flag"

// // MUST HAVE - Messaging
// #define E411 ":No recipient given (<command>)"

// #define E404 "<channel name> :Cannot send to channel"

// // MUST HAVE - Success responses
// RPL_NAMREPLY          (S353)
// RPL_ENDOFNAMES        (S366)
// RPL_TOPIC             (S332)


// Simple Rule for Basic Server:
// Numeric replies (001-599): Use :server.com prefix

// Command acknowledgments: No prefix

// Error messages: Use :server.com prefix

// Client-to-client messages: Use :nickname!user@host prefix