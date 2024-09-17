#include "../Includes/Client.hpp"

Client::Client() : _fd(0), _ip(""), _isValidPass(false), _isIdentified(false)
{
    return;
}

Client::Client(int fd, string ip) : _fd(fd), _ip(ip), _isValidPass(false), _isIdentified(false)
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
    this->_ip = cpy._ip;
    this->_isValidPass = cpy._isValidPass;
    this->_isIdentified = cpy._isIdentified;
    this->_buffer = cpy._buffer;
    return *this;
}

Client::~Client()
{
    return;
}

void Client::set_ip(int newIp)
{
    this->_ip = newIp;
}

string const &Client::get_ip()
{
    return this->_ip;
}

void Client::push_back_charBuffer(char c)
{
    this->_charBuffer.push_back(c);
}

vector<char> const &Client::get_charBuffer()
{
    return this->_charBuffer;
}

void Client::assign_buffer()
{
    this->_buffer.assign(this->_charBuffer.begin(), this->_charBuffer.end());
}

void Client::charBuffer_clear()
{
    this->_charBuffer.clear();
}

void Client::set_buffer(string newBuffer)
{
    this->_buffer = newBuffer;
}

string const &Client::get_buffer()
{
    return this->_buffer;
}

void Client::set_to_send(string const &newToSend)
{
    this->_toSend = newToSend;
}

string const &Client::get_to_send()
{
    return this->_toSend;
}

void Client::set_NICK(string const &newNICK)
{
    this->_NICK = newNICK;
}

string const &Client::get_NICK()
{
    return this->_NICK;
}

void Client::set_USER(string const &newUSER)
{
    this->_USER = newUSER;
}

string const &Client::get_USER()
{
    return this->_USER;
}

void Client::set_is_valid_pass(bool newIsValidPass)
{
    this->_isValidPass = newIsValidPass;
}

bool const &Client::get_is_valid_pass()
{
    return this->_isValidPass;
}

void Client::set_is_identified(bool newIsIdentified)
{
    this->_isIdentified = newIsIdentified;
}

bool const &Client::get_is_identified()
{
    return this->_isIdentified;
}
