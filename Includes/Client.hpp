#pragma once

#include "ft_irc.hpp"

class Client
{
private:
    int _fd;
    bool _isIdentified;

    std::string _connectionLog;
    std::string _buffer;
    std::string _NICK;
    std::string _USER;
    std::string _toSend;

    void first_connection(std::string const &password, std::map<int, Client> const &serv);
    bool capability_negotiation(std::vector<std::string> const &words);
    bool verify_password(std::vector<std::string> const &words, std::string const &password);
    bool verify_nick(std::vector<std::string> const &words, std::map<int, Client> const &serv);
    bool verify_user(std::vector<std::string> const &words);

public:
    Client();
    Client(int fd);
    Client(Client const &to_cpy);
    Client &operator=(Client const &cpy);
    ~Client();

    std::string const &get_buffer();
    void set_buffer(std::string newBuffer);

    std::string const &get_toSend();
    void set_toSend(std::string const &newToSend);

    std::string const &get_USER();

    void handle_request(std::string const &password, std::map<int, Client> const &serv);
};