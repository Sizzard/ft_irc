#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <csignal>
#include <ctime>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <utility>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
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

#define BUFFER_SIZE 1024

#define CLIENT_ID events[i].data.fd
#define CLIENT this->_clients[clientFd]
#define CLIENT_SOURCE CLIENT.get_NICK() + "!" + CLIENT.get_USER() + "@" + CLIENT.get_network()

std::vector<std::string> split(std::string const &line, std::string const &to_split);
std::vector<std::string> split_first_word(std::string const &line, std::string const &to_split);
map<string, char> split_mode(string const &line);
vector<string> analyse_command(vector<string> const &v, size_t const &i);
bool ends_with(std::string const &str, std::string const &ends);
std::string get_time();
string const get_epoch_time(time_t time);

#define RPL_WELCOME() ":" + CLIENT_SOURCE + " 001 " + CLIENT.get_NICK() + " :Welcome to my IRC server Network, " + CLIENT.get_NICK() + "!" + CLIENT.get_USER() + "@" + CLIENT.get_ip() + "\r\n"
#define RPL_YOURHOST() ":" + CLIENT_SOURCE + " 002 " + CLIENT.get_NICK() + " :Your host is " + CLIENT.get_ip() + ", running version 1.0" + "\r\n"
#define RPL_CREATED() ":" + CLIENT_SOURCE + " 003 " + CLIENT.get_NICK() + " :This server was created on " + this->_creationTime + "\r\n"
#define RPL_MYINFO() ":" + CLIENT_SOURCE + " 004 " + CLIENT.get_NICK() + " :Version 1.0 itkol" + "\r\n"
#define RPL_UMODEIS() ":" + CLIENT_SOURCE + " 221 " + CLIENT.get_NICK() + " + " + "\r\n"
#define RPL_CHANNELMODEIS(channelName) ":" + CLIENT_SOURCE + " 324 " + CLIENT.get_NICK() + " " + channelName + " " + this->_channels[channelName].get_mode() + "\r\n"
#define RPL_CREATIONTIME(channelName) ":" + CLIENT_SOURCE + " 329 " + CLIENT.get_NICK() + " " + channelName + " " + get_epoch_time(this->_channels[channelName].get_creationTime()) + "\r\n"
#define RPL_NOTOPIC(channelName) ":" + CLIENT_SOURCE + " 331 " + CLIENT.get_NICK() + " " + channelName + " :No topic is set" + "\r\n"
#define RPL_TOPIC(channelName) ":" + CLIENT_SOURCE + " 332 " + CLIENT.get_NICK() + " " + channelName + " :" + this->_channels[channelName].get_topic() + "\r\n"
#define RPL_NAMREPLY() ":" + CLIENT_SOURCE + " 353 " + CLIENT.get_NICK() + " = " + channelsToJoin[i] + " :" + this->_channels[channelsToJoin[i]].append_all_users() + "\r\n"
#define RPL_ENDOFNAMES() ":" + CLIENT_SOURCE + " 366 " + CLIENT.get_NICK() + " " + channelsToJoin[i] + " :End of /NAMES list.\r\n"
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
#define ERR_NOSUCHNICK(channelName) ":" + CLIENT_SOURCE + " 401 " + channelName + " :No such nick/channel\r\n"
#define ERR_UNKNOWNCOMMAND() ":" + CLIENT_SOURCE + " 421 " + CLIENT.get_NICK() + " :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN() ":" + CLIENT_SOURCE + " 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME() ":" + CLIENT_SOURCE + " 431 :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE() ":" + CLIENT_SOURCE + " 433 :Nickname is already in use\r\n"
#define ERR_NOTONCHANNEL() ":" + CLIENT_SOURCE + " 442 " + channel[0] + " :You're not on that channel" + "\r\n"
#define ERR_NEEDMOREPARAMS() ":" + CLIENT_SOURCE + " 461 :Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH() ":" + CLIENT_SOURCE + " 464 :Password incorrect\r\n"
#define ERR_USERSDONTMATCH() ":" + CLIENT_SOURCE + " 502 " + CLIENT.get_NICK() + " :Cant change mode for other users\r\n"