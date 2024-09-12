#include "../Includes/Client.hpp"

Client::Client() : _fd(0), _isIdentified(false)
{
    return;
}

Client::Client(int fd) : _fd(fd), _isIdentified(false)
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
    return *this;
}

Client::~Client()
{
    return;
}

void Client::set_buffer(std::string newBuffer)
{
    this->_buffer = newBuffer;
}

std::string const &Client::get_buffer()
{
    return this->_buffer;
}

void Client::set_toSend(std::string const &newToSend)
{
    this->_toSend = newToSend;
}

std::string const &Client::get_toSend()
{
    return this->_toSend;
}

std::string const &Client::get_USER()
{
    return this->_USER;
}

bool Client::capability_negotiation(std::vector<std::string> const &words)
{
    if (words.size() < 2 || words[0] != "CAP" || words[1] != "LS")
    {
        return FAILURE;
    }

    std::cout << "CAP LS OK" << std::endl;

    return SUCCESS;
}

bool Client::verify_password(std::vector<std::string> const &words, std::string const &password)
{
    if (words.size() != 2)
        return FAILURE;

    // std::cout << "PASS needs to be : " << password << " and is : " << words[1] << std::endl;

    if (words[0] == "PASS" && words[1] == password)
    {
        std::cout << "VALID PASS" << std::endl;
        return SUCCESS;
    }

    return FAILURE;
}

bool Client::verify_nick(std::vector<std::string> const &words, std::map<int, Client> const &serv)
{
    if (words.size() != 2)
        return FAILURE;
    if (words[0] != "NICK")
        return FAILURE;

    for (std::map<int, Client>::const_iterator it = serv.begin(); it != serv.end(); it++)
    {
        if (it->second._NICK == words[1])
            return FAILURE;
    }

    // std::cout << "NICK given : " << words[1] << std::endl;

    return SUCCESS;
}

bool Client::verify_user(std::vector<std::string> const &words)
{
    if (words.size() < 5)
    {
        std::cout << "Wrong USER" << std::endl;
        return FAILURE;
    }
    if (words[0] == "USER")
    {
        this->_USER = words[1];
        std::cout << "USER is : " << this->_USER << std::endl;
    }
    return SUCCESS;
}

void Client::first_connection(std::string const &password, std::map<int, Client> const &serv)
{
    this->_connectionLog += this->_buffer;

    this->_buffer.clear();

    std::vector<std::string> v = split(this->_connectionLog, "\r\n");
    std::cout << "Vector size is : " << v.size() << std::endl;

    // for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
    // {
    //     std::cout << *it << std::endl;
    // }

    if (v.size() < 4)
        return;
    if (capability_negotiation(split(v[0], " ")) == FAILURE)
    {
        this->_toSend = ":Not enough parameters";
    }
    else if (verify_password(split(v[1], " "), password) == FAILURE)
    {
        this->_toSend = ":Password incorrect";
    }
    else if (verify_nick(split(v[2], " "), serv) == FAILURE)
    {
        this->_toSend = ":No nickname given";
    }
    else if (verify_user(split(v[3], " ")) == FAILURE)
    {
        this->_toSend = ":Not enough parameters";
    }
    else
    {
        this->_toSend = "Connection successfull !";
        this->_connectionLog.clear();
        this->_isIdentified = true;

        return;
    }
    this->_connectionLog.clear();
}

void Client::handle_request(std::string const &password, std::map<int, Client> const &serv)
{
    if (this->_isIdentified == false)
        first_connection(password, serv);
    else
    {
    }
}