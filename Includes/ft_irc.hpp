#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <csignal>
#include <ctime>
#include <cerrno>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <climits>
#include <algorithm>

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

#define RED string("\033[1;31m")
#define GREEN string("\033[1;32m")
#define YELLOW string("\033[1;33m")
#define BLUE string("\033[1;34m")
#define CYAN string("\033[1;36m")
#define MAGENTA string("\033[1;35m")
#define RESET string("\033[0m")

#define SUCCESS 0
#define FAILURE 1

#define BUFFER_SIZE 512
#define CHAN_LIMIT 100
#define CLIENT_ID events[i].data.fd
#define CLIENT this->_clients[clientFd]
#define CLIENT_SOURCE CLIENT.get_NICK() + "!" + CLIENT.get_USER() + "@" + CLIENT.get_network()

#define CHANNEL(channelName) this->_channels[channelName]

typedef vector<pair<char, char> > vec_pair;

std::vector<std::string> split(std::string const &line, std::string const &to_split);
std::vector<std::string> split_first_word(std::string const &line, std::string const &to_split);
vec_pair split_mode(string line);
vector<string> analyse_command(vector<string> const &v, size_t const &i);
bool ends_with(std::string const &str, std::string const &ends);
std::string get_time();
string const get_epoch_time(time_t time);



#define RPL_WELCOME() ":" + CLIENT_SOURCE + " 001 " + CLIENT.get_NICK() + " :Welcome to my IRC server Network, " + CLIENT.get_NICK() + "!" + CLIENT.get_USER() + "@" + CLIENT.get_ip() + "\r\n"
#define RPL_YOURHOST() ":" + CLIENT_SOURCE + " 002 " + CLIENT.get_NICK() + " :Your host is " + CLIENT.get_ip() + ", running version 1.0" + "\r\n"
#define RPL_CREATED() ":" + CLIENT_SOURCE + " 003 " + CLIENT.get_NICK() + " :This server was created on " + this->_creationTime + "\r\n"
#define RPL_MYINFO() ":" + CLIENT_SOURCE + " 004 " + CLIENT.get_NICK() + " :Version 1.0 itkol" + "\r\n"
#define RPL_ISSUPORT() ":" + CLIENT_SOURCE + " 005 " + CLIENT.get_NICK() + " NETWORK=IrcServ CHANTYPES=# CHANLIMIT=#:100 PREFIX=@ CASEMAPPING=ascii :are available on this server\r\n"
#define RPL_UMODEIS() ":" + CLIENT_SOURCE + " 221 " + CLIENT.get_NICK() + " + " + "\r\n"
#define RPL_CHANNELMODEIS(channelName) ":" + CLIENT_SOURCE + " 324 " + CLIENT.get_NICK() + " " + channelName + " " + this->_channels[channelName].get_mode() + "\r\n"
#define RPL_CREATIONTIME(channelName) ":" + CLIENT_SOURCE + " 329 " + CLIENT.get_NICK() + " " + channelName + " " + get_epoch_time(this->_channels[channelName].get_creationTime()) + "\r\n"
#define RPL_NOTOPIC(channelName) ":" + CLIENT_SOURCE + " 331 " + CLIENT.get_NICK() + " " + channelName + " :No topic is set" + "\r\n"
#define RPL_TOPIC(channelName) ":" + CLIENT_SOURCE + " 332 " + CLIENT.get_NICK() + " " + channelName + " :" + this->_channels[channelName].get_topic() + "\r\n"
#define RPL_NAMREPLY(channelName) ":" + CLIENT_SOURCE + " 353 " + CLIENT.get_NICK() + " = " + channelName + " :" + this->_channels[channelName].append_all_users() + "\r\n"
#define RPL_ENDOFNAMES(channelName) ":" + CLIENT_SOURCE + " 366 " + CLIENT.get_NICK() + " " + channelName + " :End of /NAMES list.\r\n"
#define RPL_MOTDSTART() ":" + CLIENT_SOURCE + " 375 " + CLIENT.get_NICK() + " :- " + CLIENT.get_ip() + " Message of the day - \r\n"
#define RPL_MOTD() ":" + CLIENT_SOURCE + " 372 " + CLIENT.get_NICK() + " :- \r\n" +                                \
                       CYAN + "             ⣀⣤⣤⣤⣤⣄⣀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⢿⣷⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡙⠻⣿⣷⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠈⠻⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "      ⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿Merci⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣹⣿⣿⣿⣦⡀                 \r\n" + RESET + \
                       CYAN + "⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿a⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣶⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿Leo⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡧⣼⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿pour⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣽⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿son⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣷⡈⢻⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿aide⣿⣿⡇⢸⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡌⠻⣦⡽⣆⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸⣧⠀⠹⣿⡎⢿⠙⢿⣿⣿⣿⡟⢿⣿⣿⣿⣿⣿⣿⣿⣿⣄⠈⠻⣾⣆⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠏⢺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⢸⡟⠀⠀⠸⣧⠈⣇⠀⠹⣿⣿⣿⣬⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠈⢻⡄⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⡿⠁⢸⡇⠀⠀⠀⢻⡀⢸⣤⠞⠛⣿⠻⡇⠸⣿⣿⡟⠛⣿⣿⡟⣷⠙⣆⠀⠀⠙⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣽⣿⡿⢁⣿⣿⣿⠃⠀⢸⠇⠀⠀⠀⢨⠟⠉⠀⠀⠀⠘⡆⠙⠀⢿⢻⣿⡜⣿⢹⡇⠘⣇⠈⠑⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⡿⣿⣿⠋⠙⣿⣿⠏⠒⠉⡾⠀⠀⠀⢀⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀⠘⠈⢿⡧⣿⠀⣇⠀⠘⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⠃⠀⢻⡏⢀⣴⣿⣟⡒⣶⣴⠃⠀⠀⠀⠸⠛⠛⢃⡤⢄⣀⣀⣀⣀⣤⡄⠀⣼⣷⠃⠀⠘⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⣸⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⢸⡇⠀⠀⢸⡇⠉⠀⠟⠉⠙⠛⠋⠀⠀⠀⠀⠀⠀⠀⣿⡟⢿⣿⣍⣽⠀⢺⠁⠀⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠈⢻⡏⠉⡉⠻⣿⣿⣿⣿⡏⢻⣟⠇⢸⠃⠀⠀⠀⠁⣠⣴⣶⣶⡒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢷⡈⠛⠟⢃⣠⠞⠀⢸⡟⠓⠤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⣷⡇⠙⡷⢼⣿⣿⣿⠁⢈⡿⠄⠀⣠⠤⠴⣶⣿⣏⣹⠛⣯⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠀⡀⠀⣾⡀⢢⠀⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⢿⡇⠀⡇⠀⢈⡿⣿⡄⠸⣇⠀⠀⠛⢦⣄⠈⢛⣟⣫⠴⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠃⠉⠀⠀⢹⢧⡼⠀⢰⠷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠘⣿⡇⠃⠀⢿⣀⡘⢧⡀⠹⣆⠀⠀⠀⠀⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣆⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡾⠁⠀⠈⠀⡼⢟⠒⠤⣀⡀⠀⠀⠀⠀⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠈⠛⠲⢤⣄⠈⢁⠀⠁⠀⡹⣦⠀⠀⠀⢘⣆⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⠆⠀⠀⠀⠀⠀⠀⠀⣠⠟⠀⠀⠀⠀⡼⡇⠈⢧⠀⠈⠁⠀⠀⠀⠀⠀⠤\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣹⠿⠚⠛⠉⠉⠉⠉⠉⠻⠯⠥⠴⣋⣸⠀⠀⠀⠀⠀⠀⠀⠙⠿⠿⠁⠀⠀⠀⠀⠀⠀⠀⡰⠋⠀⠀⠀⠀⢸⠁⣿⠀⠈⢳⠀⠀⠀⣴⠛⠁⠀⠀\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣯⠶⠖⠒⠛⠉⠉⣙⠳⠶⠶⢶⣾⡿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣤⠀⠀⠀⠀⠀⢠⠞⠁⠀⠀⢀⡀⠀⡎⠀⣏⠀⠀⠀⠀⠀⠀⠈⢷⡀⢠⣾\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠏⠀⠀⠀⢀⣴⣾⠟⢛⢿⣋⠉⢧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠴⣶⡿⠟⠃⠀⠀⠀⢀⡴⠃⠀⠀⠀⢠⡟⠀⢸⡇⠀⡿⠀⠀⠀⠀⠀⠀⠀⠈⣷⣸⣿\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠀⠀⠀⡾⠀⠀⠀⠀⣸⡟⠁⠀⡞⠀⠉⠳⢦⣉⣻⣦⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠟⠀⠀⠀⠀⠀⠀⡇⠀⢸⠁⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⠀⠀⠀⠀⡼⠃⠀⠀⠀⣰⡟⠀⠀⡼⠁⠀⠀⠀⣠⠞⠉⢀⣀⣄⠀⢹⡆⠀⠀⠀⠀⠀⠀⢰⡷⠛⠒⠒⢤⣀⡀⠀⢰⠀⠀⡌⠀⢰⠃⠀⠀⠀⠀⠀⠀⣠⣾⢿⣿⣿\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠀⣀⡤⠖⠚⠙⠚⠛⠳⠶⠿⠧⠤⠴⢥⣤⣀⠀⡼⢡⡶⠛⠉⣠⠞⣴⡾⠯⣶⣤⣀⠀⢀⣠⡾⣷⠀⠀⠀⠀⢸⠁⢠⠊⠀⠀⡇⠓⡇⠀⠀⠀⠀⠀⣠⡾⠋⢠⣿⣿⣿\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⢀⣧⠖⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⣧⡟⠀⠀⣼⢿⡾⠉⠛⣦⡀⠀⠈⠉⠉⠀⠀⢞⡆⠀⠀⠀⡜⠀⠁⠀⠀⠀⡇⢰⡁⠀⠀⠀⡠⠚⠉⠳⣄⣬⣿⣿⣿\r\n" + RESET +     \
                       CYAN + "⠀⠀⠀⠉⠁⠀⠀⠛⠶⢤⣤⣤⣴⣖⣒⡒⠲⣤⣄⠀⢀⡏⠀⠀⢸⣇⡾⠁⠀⠀⠀⠙⢶⡀⠀⠀⠀⠀⠸⣿⠀⠀⢸⡇⠀⠀⠀⠀⢸⠁⠀⠉⢲⡴⠊⠀⠀⠀⠀⠈⠣⡽⣿⣿\r\n" + RESET +     \
                       CYAN + "⡀⠀⠀⠀⠀⠒⠂⠆⠀⠀⠀⠀⠀⠈⠉⠉⠓⠲⣬⣦⣟⡁⠀⠀⣨⣿⣿⣦⡚⣻⣤⠀⠀⢻⡆⠀⠀⠀⢰⣿⠀⠀⠀⠹⡄⠀⠀⠀⢸⠀⣀⡴⠋⠀⠀⠀⠀⠀⠀⠀⠀⠁⢾⣿\r\n" + RESET +
#define RPL_ENDOFMOTD() ":" + CLIENT_SOURCE + " 376 " + CLIENT.get_NICK() + " :End of /MOTD\r\n"
#define RPL_INVITING(nick, channelName) ":" + CLIENT_SOURCE + " 341 " + CLIENT.get_NICK() + " " + nick + " " + channelName + "\r\n"
#define ERR_UNKNOWNERROR(message) ":" + CLIENT_SOURCE + " 400 " + CLIENT.get_NICK() + " :" + message + "\r\n"
#define ERR_NOSUCHNICK(channelName) ":" + CLIENT_SOURCE + " 401 " + channelName + " :No such nick/channel\r\n"
#define ERR_UNKNOWNCOMMAND() ":" + CLIENT_SOURCE + " 421 " + CLIENT.get_NICK() + " :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN() ":" + CLIENT_SOURCE + " 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME() ":" + CLIENT_SOURCE + " 431 :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE() ":" + CLIENT_SOURCE + " 433 :Nickname is already in use\r\n"
#define ERR_USERNOTINCHANNEL(channelName, nick) ":" + CLIENT_SOURCE + " 441 " + CLIENT.get_NICK() + " " + nick + " " + channelName + " :They aren't on that channel\r\n"
#define ERR_NEEDMOREPARAMS() ":" + CLIENT_SOURCE + " 461 " + CLIENT.get_NICK() + " :Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH() ":" + CLIENT_SOURCE + " 464 :Password incorrect\r\n"
#define ERR_BADCHANNELKEY(channelName) ":" + CLIENT_SOURCE + " 475 " + CLIENT.get_NICK() + " " + channelName + " :Cannot join channel (+k)\r\n"
#define ERR_USERSDONTMATCH() ":" + CLIENT_SOURCE + " 502 " + CLIENT.get_NICK() + " :Cant change mode for other users\r\n"
#define ERR_INVALIDMODEPARAM(channelName, args) ":" + CLIENT_SOURCE + " 696 " + CLIENT.get_NICK() + " " + channelName + " +l " + args + " :Bad input\r\n"
#define ERR_CHANOPRIVSNEEDED() ":" + CLIENT_SOURCE + " 482 " + CLIENT.get_NICK() + " :You're not a channel operator\r\n"
#define ERR_NOSUCHCHANNEL(channelName) ":" + CLIENT_SOURCE + " 403 " + channelName + " :No such channel\r\n"
#define ERR_NOTONCHANNEL(channelName) ":" + CLIENT_SOURCE + " 442 " + channelName + " :You're not on that channel" + "\r\n"
#define ERR_USERONCHANNEL(channelName) ":" + CLIENT_SOURCE + " 443 " + channelName + " :is already on channel\r\n"
#define ERR_CANNOTSENDTOCHAN(channelName) ":" + CLIENT_SOURCE + " 404 " + channelName + " :Cannot send to channel\r\n"
#define RPL_CHANGE_NICK(newNick) ":" + CLIENT_SOURCE + " NICK " + newNick + "\r\n"
#define ERR_CHANNELISFULL(channelName) ":" + CLIENT_SOURCE + " 471 " + CLIENT.get_NICK() + " " + channelName + " :Cannot join channel (+l)\r\n"
#define ERR_INVITEONLYCHAN(channelName) ":" + CLIENT_SOURCE + " 473 " + CLIENT.get_NICK() + " " + channelName + " :Cannot join channel (+i)\r\n"