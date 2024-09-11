#include "Includes/Client.hpp"

Client::Client() : _fd(0), _isIdentified(false), _negotiating(false)
{
    return;
}

Client::Client(int fd) : _fd(fd), _isIdentified(false), _negotiating(false)
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
    this->_buffer = cpy._buffer;
    this->_isIdentified = cpy._isIdentified;
    this->_negotiating = cpy._negotiating;
    return *this;
}

Client::~Client()
{
    return;
}

std::string const &Client::get_buffer()
{
    return this->_buffer;
}

void Client::set_buffer(std::string newBuffer)
{
    this->_buffer += newBuffer;
}

bool Client::capability_negotiation(std::vector<std::string> const &words)
{
    if (words.size() < 2 || words[0].compare("CAP") != 0 || words[1].compare("LS") != 0)
    {
        return FAILURE;
    }
    this->_negotiating = true;
    return SUCCESS;
}

bool Client::verify_password()
{
    ;
    return true;
}

void Client::handle_request()
{
    std::vector<std::string> v = split(this->_buffer, "\r\n");

    for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
    {
        std::vector<std::string> word = split(*it, " ");

        if (this->_isIdentified == false && capability_negotiation(word) == FAILURE)
            return;
        else
        {
            if (verify_password() == true)
                ;
        }
    }

    this->_buffer.clear();
}