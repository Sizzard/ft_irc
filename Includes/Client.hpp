#pragma once

#include "ft_irc.hpp"

class Client
{
private:
    int _fd;
    std::string _ip;
    bool _isValidPass;
    bool _isIdentified;

    std::string _buffer;
    std::string _NICK;
    std::string _USER;
    std::string _toSend;

    void first_connection(std::string const &password, std::map<int, Client> const &serv);
    void capability_negotiation(std::vector<std::string> const &words);
    void verify_password(std::vector<std::string> const &words, std::string const &password);
    void verify_nick(std::vector<std::string> const &words, std::map<int, Client> const &serv);
    void verify_user(std::vector<std::string> const &words);

public:
    Client();
    Client(int fd, std::string ip);
    Client(Client const &to_cpy);
    Client &operator=(Client const &cpy);
    ~Client();

    void set_buffer(std::string newBuffer);
    std::string const &get_buffer();

    void set_toSend(std::string const &newToSend);
    std::string const &get_toSend();

    std::string const &get_USER();

    bool handle_request(std::string const &password, std::map<int, Client> const &serv);
};

#define RPL_WELCOME() "001 " + this->_NICK + " :Welcome to my IRC server Network, " + this->_NICK + "!" + this->_USER + "@" + this->_ip + "\r\n"
#define RPL_YOURHOST() "002 " + this->_NICK + " :Your host is " + this->_ip + ", running version 1.0" + "\r\n"
#define RPL_CREATED() "003 " + this->_NICK + " :This server was created on " + get_time() + "\r\n"
#define RPL_MYINFO() "004 " + this->_NICK + " :Version 1.0 itkol" + "\r\n"
#define RPL_MOTDSTART() "375 " + this->_NICK + " :- " + this->_ip + " Message of the day - \r\n"
#define RPL_MOTD() "372 " + this->_NICK + " :- " + cyan + "Welcome to my IRC server Network, " + this->_NICK + "!" + this->_USER + "@" + this->_ip + reset + "\r\n"
#define RPL_ENDOFMOTD() "376 " + this->_NICK + " :End of MOTD command\r\n"

#define ERR_UNKNOWNCOMMAND() "421 :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN() "431 :No nickname given\r\n"
#define ERR_NICKNAMEINUSE() "433 :Nickname is already in use\r\n"
#define ERR_NEEDMOREPARAMS() "461 :Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH() "464 :Password incorrect\r\n"