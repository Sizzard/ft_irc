#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <csignal>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

using std::cout;
using std::endl;
using std::map;
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

#define CLIENT_ID events[i].data.fd

std::vector<std::string> split(std::string const &line, std::string const &to_split);
bool ends_with(std::string const &str, std::string const &ends);
std::string get_time();

#define RPL_WELCOME() "001 " + this->_NICK + " :Welcome to my IRC server Network, " + this->_NICK + "!" + this->_USER + "@" + this->_ip + "\r\n"
#define RPL_YOURHOST() "002 " + this->_NICK + " :Your host is " + this->_ip + ", running version 1.0" + "\r\n"
#define RPL_CREATED() "003 " + this->_NICK + " :This server was created on " + get_time() + "\r\n"
#define RPL_MYINFO() "004 " + this->_NICK + " :Version 1.0 itkol" + "\r\n"
#define RPL_MOTDSTART() "375 " + this->_NICK + " :- " + this->_ip + " Message of the day - \r\n"
#define RPL_MOTD() "372 " + this->_NICK + " :- " + CYAN + "Welcome to my IRC server Network, " + this->_NICK + "!" + this->_USER + "@" + this->_ip + RESET + "\r\n"
#define RPL_ENDOFMOTD() "376 " + this->_NICK + " :End of MOTD command\r\n"

#define ERR_UNKNOWNCOMMAND() "421 :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN() "431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME() "431 :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE() "433 :Nickname is already in use\r\n"
#define ERR_NEEDMOREPARAMS() "461 :Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH() "464 :Password incorrect\r\n"