/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:48:29 by mtelek            #+#    #+#             */
/*   Updated: 2025/10/21 21:27:28 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// 400-499: Client errors
#define E401 "<nickname> :No such nick/channel"
#define E402 "<server name> :No such server"
#define E403 "<channel name> :No such channel"
#define E404 "<channel name> :Cannot send to channel"
#define E405 "<channel name> :You have joined too many channels"
#define E406 "<nickname> :There was no such nickname"
#define E407 "<target> :Duplicate recipients. No message delivered"
#define E409 ":No origin specified"

// Registration errors
#define E431 ":No nickname given"
#define E432 "<nick> :Erroneous nickname"
#define E433 " :Nickname is already in use"
#define E436 "<nick> :Nickname collision KILL"

// Authentication errors  
#define E461 "<command> :Not enough parameters"
#define E462 ":You may not reregister"
#define E464 ":Password incorrect"
#define E465 ":You are banned from this server"

// Channel errors
#define E442 "<channel> :You're not on that channel"
#define E443 "<user> <channel> :is already on channel"
#define E451 ":You have not registered"
#define E471 "<channel> :Cannot join channel (+l)"
#define E473 "<channel> :Cannot join channel (+i)"
#define E474 "<channel> :Cannot join channel (+b)"
#define E475 "<channel> :Cannot join channel (+k)"
#define E476 "<channel> :Bad Channel Mask"

// Success responses (001-099)
#define E001 ":Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define E002 ":Your host is <servername>, running version <ver>"
#define E003 ":This server was created <date>"
#define E004 "<servername> <version> <available user modes> <available channel modes>"

// MOTD (Message of the Day)
#define E375 ":- <server> Message of the Day - "
#define E372 ":- <text>"
#define E376 ":End of /MOTD command"

// Channel responses
#define E353 "<channel> :[[@|+]<nick> [[@|+]<nick> ...]]"
#define E366 "<channel> :End of /NAMES list"
#define E332 "<channel> :<topic>"
#define E333 "<channel> <nick> <time>"

// Who/User information
#define E352 "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"
#define E315 "<name> :End of /WHO list"
#define E311 "<nick> <user> <host> * :<real name>"
#define E312 "<nick> <server> :<server info>"
#define E318 "<nick> :End of /WHOIS list"

// PRIVMSG/NOTICE errors
#define E411 ":No recipient given (<command>)"
#define E412 ":No text to send"

// MODE errors
#define E472 "<char> :is unknown mode char to me"
#define E502 ":Cant change mode for other users"
#define E501 ":Unknown MODE flag"


// //MUST HAVE - Registration
// #define E461 "<command> :Not enough parameters"
// #define E462 ":You may not reregister"
// #define E464 ":Password incorrect"
// #define E431 ":No nickname given"
// #define E432 "<nick> :Erroneous nickname"
// #define E433 "<nick> :Nickname is already in use"
// #define E451 ":You have not registered"

// // MUST HAVE - Channel operations
// #define E403 "<channel name> :No such channel"
// #define E442 "<channel> :You're not on that channel"
// #define E443 "<user> <channel> :is already on channel"
// #define E471 "<channel> :Cannot join channel (+l)"
// #define E473 "<channel> :Cannot join channel (+i)"
// #define E475 "<channel> :Cannot join channel (+k)"

// // MUST HAVE - Messaging
// #define E411 ":No recipient given (<command>)"
// #define E412 ":No text to send"
// #define E404 "<channel name> :Cannot send to channel"

// // MUST HAVE - Success responses
// RPL_WELCOME           (S001)
// RPL_YOURHOST          (S002) 
// RPL_CREATED           (S003)
// RPL_MYINFO            (S004)
// RPL_NAMREPLY          (S353)
// RPL_ENDOFNAMES        (S366)
// RPL_TOPIC             (S332)