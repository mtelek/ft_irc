/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtelek <mtelek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:48:29 by mtelek            #+#    #+#             */
/*   Updated: 2025/11/21 23:31:46 by mtelek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//AUTHENTICATE PASSWORD
#define E464(serv,nick)  serv + " 464 " + nick + " :Password incorrect\r\n"
#define E4642(serv,nick) serv + " 4642 " + nick + " :Password incorrect. Disconnecting\r\n"
#define S467(serv,nick) serv + " 467 " + nick + " :Authentication successful\r\n"
#define E462(serv,nick) serv + " 462 " + nick + " :You may not reregister\r\n"
#define ERR_NOTREGISTERED(serv, nick) serv + " 451 " + nick + " :You have not registered\r\n"

//SETNICK
#define E433(serv,nick) serv + " 433 " + nick + " :Nickname is already in use\r\n"
#define E431(serv,nick) serv + " 431 " + nick + " :No nickname given\r\n"
#define S435(serv, nick) serv + " 435 " + nick + " :Nickname set to " + nick + " \r\n"
#define E4643(serv,nick) serv + " 4643 " + nick + " :Authentication required\r\n"
#define E432(serv,nick) serv + " 432 " + nick + " :Erroneous nickname\r\n"
#define E4322(serv,nick) serv + " 4322 " + nick + " :Warning: Erroneous nickname\r\n"

//QUIT
#define S410(serv,nick) serv + " 410 " + nick +" :Disconnected\r\n"

//SETUSER
#define S411(serv, nick, user) serv + " 411 " + nick + " :Username set to " + user + "\r\n"
#define S412(serv, nick, real) serv + " 412 " + nick + " :Realname set to " + real + "\r\n"
#define E4323(serv,nick) serv + " 432 " + nick + " :Erroneous username or realname\r\n"

//EXECUTE COMMANDDS
#define E461(serv, nick, cmd) serv + " 461 " + nick + " :" + cmd + " :Not enough parameters\r\n"
#define E463(serv, nick, cmd) serv + " 463 " + nick + " :" + cmd + " :Invalid command\r\n"

//SENDPRIVATE
#define S465(serv,nick) serv + " 465 " + nick + " :Message sent\r\n"
#define E412(serv,nick) serv + " 412 " + nick + " :No text to send\r\n"
#define E401(serv, nick, target) serv + " 401 " + nick + " :" + target + " :No such nick\r\n"

//CLIENT WELCOME MESSAGE
#define RPL_WELCOME(serv, nick, server, user, host) serv + " 001 " + nick + " :Welcome to the " + server + " Network, " + nick + "[!" + user + "@" + host + "]\r\n"
#define RPL_YOURHOST(serv, nick, server, version) serv + " 002 " + nick + " :Your host is " + server + " running version " + version + "\r\n"
#define RPL_CREATED(serv, nick, date) serv + " 003 " + nick + " :This server was created " + date + "\r\n"
#define RPL_MYINFO(serv, nick, server, version) serv + " 004 " + nick + " :" + server + " " + version + " 0 3\r\n"
#define RPL_ISUPPORT(serv, nick) serv + " 005 " + nick + " " + serv + " :CASEMAPPING=rfc1459 CHANTYPES=# CHANMODES=itl PREFIX=(ov)@+ CHANLIMIT=#: MAXNICKLEN=9 NICKLEN=9 TOPICLEN=307 KICKLEN=307 CHANNELLEN=200 USERLEN=9\r\n"

//INVITE - CHANNEL
#define ERR_NEEDMOREPARAMS(serv, nick, command) serv + " 461 " + nick + " " + command + " :Not enough parameters\r\n"
#define ERR_TOOMANYPARAMS_I(serv, nick) serv + " 463 " + nick + " :Too many parameters. Correct usage: INVITE <nickname> <#channel\r\n"
#define ERR_TOOMANYPARAMS_J(serv, nick) serv + " 463 " + nick + " :Too many parameters. Correct usage: JOIN <#channel> <(optional)password>\r\n"
#define ERR_TOOMANYTARGETS(serv, nick) serv + " 407 " + nick + " :Multiple targets cannot be invited at once\r\n"
#define ERR_TOOMANYCHAN(serv, nick) serv + " 408 " + nick + " :Cannot send multiple channel invitations at once\r\n"
#define ERR_NOSUCHCHANNEL(serv, nick) serv + " 403 " + nick + " :No such channel\r\n"
#define ERR_NOTONCHANNEL(serv, nick, channel)  serv + " 442 " + nick + " " + channel + " :You're not on that channel\r\n"
#define ERR_CHANOPRIVSNEEDED(serv, nick, channel) serv + " 482 " + nick + " " + channel + " :You're not channel operator\r\n"
#define ERR_USERONCHANNEL(serv, nick, channel) serv + " 443 " + nick + " " + channel + " :is already on channel\r\n"

//JOIN - CHANNEL
#define ERR_INVITEONLYCHAN(serv, nick, channel) serv + " 473 " + nick + " " + channel + " :Cannot join channel (+i) - you must be invited\r\n"
#define ERR_CHANNELISFULL(serv, nick, channel) serv + " 471 " + nick + " " + channel + " :Cannot join channel (+l) - channel is full\r\n"
#define ERR_BADCHANNELKEY(serv, nick, channel) serv + " 475 " + nick + " " + channel + " :Cannot join channel (+k) - bad key\r\n"
#define ERR_BADCHANMASK(serv, channel) serv + " 476 " + channel + " :Bad Channel Mask\r\n"
#define ERR_CHANNELALREADYEXISTS(serv, channel) serv + " 400 " + channel + " :Channel already exists\r\n"

//KICK - CHANNEL
#define ERR_USERNOTINCHANNEL(serv, nick, target, channel) serv + " 441 " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n"


// // 400-499: Client errors
// #define E403 "<channel name> :No such channel"
// #define E404 "<channel name> :Cannot send to channel"
// #define E405 "<channel name> :You have joined too many channels"
// #define E406 "<nickname> :There was no such nickname"
// #define E407 "<target> :Duplicate recipients. No message delivered"
// #define E409 ":No origin specified"
// #define E451 ":You have not registered"
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

// // MODE errors
// #define E472 "<char> :is unknown mode char to me"
// #define E502 ":Cant change mode for other users"
// #define E501 ":Unknown MODE flag"

// // MUST HAVE - Messaging
// #define E411 ":No recipient given (<command>)"

// // MUST HAVE - Success responses
// RPL_NAMREPLY          (S353)
// RPL_ENDOFNAMES        (S366)
// RPL_TOPIC             (S332)