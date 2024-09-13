#include "../Includes/Client.hpp"

Client::Client() : _fd(0), _ip(""), _isValidPass(false), _isIdentified(false)
{
    return;
}

Client::Client(int fd, std::string ip) : _fd(fd), _ip(ip), _isValidPass(false), _isIdentified(false)
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

void Client::capability_negotiation(std::vector<std::string> const &words)
{
    if (words.size() < 2)
    {
        this->_toSend = ERR_NEEDMOREPARAMS();
        return;
    }
    else if (words[0] == "CAP" && words[1] == "LS")
    {
        std::cout << "CAP LS OK" << std::endl;
        return;
    }
    this->_toSend = ERR_NEEDMOREPARAMS();
    return;
}

void Client::verify_password(std::vector<std::string> const &words, std::string const &password)
{
    if (words.size() != 2)
    {
        this->_toSend = ERR_NEEDMOREPARAMS();
        return;
    }

    // std::cout << "PASS needs to be : " << password << " and is : " << words[1] << std::endl;

    if (words[0] == "PASS" && words[1] == password)
    {
        std::cout << "VALID PASS" << std::endl;
        this->_isValidPass = true;
        return;
    }

    this->_toSend = ERR_PASSWDMISMATCH();
    return;
}

void Client::verify_nick(std::vector<std::string> const &words, std::map<int, Client> const &serv)
{
    if (words.size() != 2)
    {
        this->_toSend = ERR_NEEDMOREPARAMS();
        return;
    }

    for (std::map<int, Client>::const_iterator it = serv.begin(); it != serv.end(); it++)
    {
        if (it->second._NICK == words[1])
        {
            this->_toSend = ERR_NICKNAMEINUSE();
            return;
        }
    }

    this->_NICK = words[1];
    std::cout << "NICK given : " << words[1] << std::endl;

    return;
}

void Client::verify_user(std::vector<std::string> const &words)
{
    if (words.size() < 5)
    {
        std::cout << "Wrong USER" << std::endl;
        this->_toSend = ERR_NEEDMOREPARAMS();
        return;
    }
    if (words[0] == "USER")
    {
        this->_USER = words[1];
        std::cout << "USER is : " << this->_USER << std::endl;
    }
    return;
}

void Client::first_connection(std::string const &password, std::map<int, Client> const &serv)
{
    std::vector<std::string> v = split(this->_buffer, "\r\n");

    // std::cout << "Vector size is : " << v.size() << std::endl;

    if (v.size() == 0)
    {
        this->_toSend = ERR_NEEDMOREPARAMS();
        return;
    }
    else
    {
        for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
        {
            std::vector<std::string> words = split(*it, " ");
            if (words.size() == 0)
            {
                this->_toSend = ERR_NEEDMOREPARAMS();
                return;
            }
            if (words[0] == "CAP")
            {
                capability_negotiation(words);
            }
            else if (words[0] == "PASS")
            {
                verify_password(words, password);
            }
            else if (words[0] == "NICK")
            {
                verify_nick(words, serv);
            }
            else if (words[0] == "USER")
            {
                verify_user(words);
            }
            else
            {
                std::cout << "Words[0] is : " << words[0] << std::endl;
                std::cout << ERR_UNKNOWNCOMMAND() << std::endl;
                this->_toSend = ERR_UNKNOWNCOMMAND();
                return;
            }
            if (this->_toSend.empty() == false)
            {
                return;
            }
        }
    }
    if (this->_isValidPass == true && this->_NICK.empty() == false && this->_USER.empty() == false)
    {
        this->_isIdentified = true;
        this->_toSend = RPL_WELCOME();
        this->_toSend += RPL_YOURHOST();
        this->_toSend += RPL_CREATED();
        this->_toSend += RPL_MYINFO();
        this->_toSend += RPL_MOTDSTART();
        this->_toSend += RPL_MOTD();
        this->_toSend += RPL_ENDOFMOTD();
    }
    return;
}

bool Client::handle_request(std::string const &password, std::map<int, Client> const &serv)
{
    if (this->_isIdentified == false)
    {
        first_connection(password, serv);
        return SUCCESS;
    }
    else
    {
    }
    return SUCCESS;
}