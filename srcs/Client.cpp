#include "../Includes/Client.hpp"

Client::Client() : _fd(0), _ip(""), _isValidPass(false), _isIdentified(false), _quit(false), _channelList(false)
{
    return;
}

Client::Client(int fd, string ip, epoll_event clientEvent) : _fd(fd), _ip(ip), _isValidPass(false), _isIdentified(false), _quit(false), _channelList(false), _event(clientEvent)
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
    this->_quit = cpy._quit;
    this->_channelList = cpy._channelList;
    this->_event = cpy._event;
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

void Client::erase_to_send(size_t pos, size_t len)
{
    this->_toSend.erase(pos, len);
}

void Client::set_to_send(string const &newToSend)
{
    this->_toSend = newToSend;
}

void Client::append_to_send(string const &newToAppend)
{
    this->_toSend += newToAppend;
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

void Client::set_network(string const &newNetwork)
{
    this->_network = newNetwork;
}

string const &Client::get_network()
{
    return this->_network;
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


void Client::add_to_channelList(string newInChannel)
{
    this->_channelList.push_back(newInChannel);
}

void Client::remove_from_channelList(string const &InChannelToRemove)
{
    vector<string>::iterator it = std::find(this->_channelList.begin(), this->_channelList.end(), InChannelToRemove);
    if (it != this->_channelList.end())
    {
        this->_channelList.erase(it);
    }
}

vector<string> const &Client::get_channelList()
{
    return this->_channelList;
}

void Client::set_quit(bool newQuit)
{
    this->_quit = newQuit;
}

bool const &Client::get_quit()
{
    return this->_quit;
}

void Client::add_epollout(int const &epoll_fd)
{
    this->_event.events |= EPOLLOUT;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->_fd, &this->_event) == -1)
        throw std::runtime_error("epoll_ctl error");
}

void Client::remove_epollout(int const &epoll_fd)
{
    this->_event.events = this->_event.events & ~EPOLLOUT;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->_fd, &this->_event) == -1)
        throw std::runtime_error("epoll_ctl error");
}

epoll_event &Client::get_epoll_event()
{
    return this->_event;
}