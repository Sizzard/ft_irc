#include "Includes/Client.hpp"

Client::Client() : _fd(0)
{
    return;
}

Client::Client(int fd) : _fd(fd)
{
    return;
}

Client::Client(Client const &cpy)
{
    *this = cpy;
    return;
}

Client &Client::operator=(Client const &cpy)
{
    this->_fd = cpy._fd;
    this->_message = cpy._message;
    return *this;
}

Client::~Client()
{
    return;
}
