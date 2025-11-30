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
#define ERR_AUTH_REQUIRED(serv, nick) serv + " 463 " + nick + " :Authentication required\r\n"
#define ERR_NOTREGISTERED(serv, nick) serv + " 451 " + nick + " :You have not registered\r\n"
#define ERR_PASS_ALREADY_GIVEN(serv, nick) serv + " 466 " + nick + " :Correct password has already been sent\r\n"

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
#define E401(serv, nick, target) serv + " 401 " + nick + " :" + target + " :No such nick\r\n"
#define ERR_NOTEXTTOSEND(serv, nick) serv + " 412 " + nick + " :No text to send\r\n"
#define ERR_NORECIPIENT(serv, nick, command) serv + " 411 " + nick + " :No recipient given (" + command + ")\r\n"

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

//TOPIC - CHANNEL
#define RPL_NOTOPIC(serv, nick, channel) serv + " 331 " + nick + " " + channel + " :No topic is set\r\n"
#define RPL_TOPIC(serv, nick, channel, topic) serv + " 332 " + nick + " " + channel + " :" + topic + "\r\n"

//MODE - CHANNEL
#define ERR_INVALIDMODEPARAM(serv, nick, channel, mode, param) serv + " 696 " + nick + " " + channel + " " + mode + " " + param + " :Invalid mode parameter\r\n"
#define ERR_USERISOPERATOR(serv, nick, channel, target) serv + " 400 " + nick + " " + target + " " + channel + " :is already a channel operator\r\n"
#define ERR_NOPRIVILEGES(server, nick) (":" + server + " 481 " + nick + " :Permission Denied-cannot remove operator rights if you are only operator on the channel\r\n")
#define ERR_CANNOTREMOVEOPER(server, nick, channel, target) (":" + server + " 481 " + nick + " " + channel + " :" + target + " is not a channel operator\r\n")
#define ERR_UNKNOWNMODE(server, nick, mode) (":" + server + " 472 " + nick + " " + mode + " :is unknown mode char to me\r\n")
