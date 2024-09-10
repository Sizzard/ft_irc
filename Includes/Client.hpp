#pragma once

#include "ft_irc.hpp"

class Client
{

public:
    Client();
    Client(int fd);
    Client(Client const &to_cpy);
    Client &operator=(Client const &cpy);
    ~Client();

    int _fd;
    std::string _message;
};