#pragma once

#include "ft_irc.hpp"

class Client
{
private:
    int _fd;
    bool _isIdentified;
    bool _negotiating;
    std::string _buffer;
    std::string _NICK;
    std::string _USER;

    bool capability_negotiation(std::vector<std::string> const &words);
    bool verify_password();

public:
    Client();
    Client(int fd);
    Client(Client const &to_cpy);
    Client &operator=(Client const &cpy);
    ~Client();

    std::string const &get_buffer();
    void set_buffer(std::string newBuffer);

    void handle_request();
};