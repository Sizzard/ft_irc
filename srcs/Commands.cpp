#include "../Includes/Server.hpp"

void Server::CAP(int const &clientFd, vector<string> const &words)
{
    if (words.size() < 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    else if (words[0] == "CAP" && words[1] == "LS")
    {
        cout << "CAP LS OK" << endl;
        return;
    }
    APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
    return;
}

void Server::PASS(int const &clientFd, vector<string> const &words)
{
    CLIENT.set_is_valid_pass(false);

    if (words.size() != 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    // cout << "PASS needs to be : " << password << " and is : " << words[1] << endl;

    if (words[0] == "PASS" && words[1] == this->_password)
    {
        cout << "VALID PASS" << endl;
        CLIENT.set_is_valid_pass(true);
        return;
    }

    APPEND_CLIENT_TO_SEND(ERR_PASSWDMISMATCH());
    return;
}

void Server::NICK(int const &clientFd, vector<string> const &words)
{
    if (words.size() != 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    if (*words[1].begin() == '#')
    {
        APPEND_CLIENT_TO_SEND(ERR_ERRONEUSNICKNAME());
        return;
    }

    for (map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (it->second.get_NICK() == words[1])
        {
            APPEND_CLIENT_TO_SEND(ERR_NICKNAMEINUSE());
            return;
        }
    }

    CLIENT.set_NICK(words[1]);
    cout << "NICK given : " << words[1] << endl;

    return;
}

void Server::USER(int const &clientFd, vector<string> const &words)
{
    if (words.size() < 5)
    {
        cout << "Wrong USER" << endl;
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    if (words[0] == "USER")
    {
        CLIENT.set_USER(words[1]);
        CLIENT.set_network(words[3]);
        cout << "USER is : " << CLIENT.get_USER() << endl;
    }
    return;
}

void Server::PING(int const &clientFd, vector<string> const &words)
{
    if (words.size() != 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
    }
    else
    {
        APPEND_CLIENT_TO_SEND("PONG " + words[1] + "\r\n");
    }
    return;
}

void Server::QUIT(int const &clientFd, vector<string> const &words)
{

    if (words.size() == 1)
    {
        APPEND_CLIENT_TO_SEND(CLIENT_SOURCE + " QUIT :\r\n");
    }
    else
    {
        APPEND_CLIENT_TO_SEND(CLIENT_SOURCE + " QUIT " + words[1] + "\r\n");
    }

    CLIENT.set_quit(true);
    return;
}

void Server::JOIN(int const &clientFd, vector<string> const &words)
{
    if (words.size() != 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    if (words[1].empty() == true)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    if (this->_channels.find(words[1]) == this->_channels.end())
    {
        this->_channels[words[1]] = Channels();
    }
    this->_channels[words[1]].add_users(clientFd, CLIENT.get_NICK());
    CLIENT.set_inChannel(true);
    APPEND_CLIENT_TO_SEND(":" + CLIENT_SOURCE + " JOIN :" + words[1] + "\r\n");
    APPEND_CLIENT_TO_SEND(RPL_NAMREPLY() + RPL_ENDOFNAMES());

    return;
}

void Server::PRIVMSG(int const &clientFd, vector<string> const &words)
{
    if (words.size() < 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    vector<string> channel_message = split_first_word(words[1], " ");

    map<string, Channels>::iterator it = this->_channels.find(channel_message[0]);
    if (it == this->_channels.end())
    {
        for (map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
        {
            if (it->second.get_NICK() == channel_message[0])
            {
                this->_clients[it->first].set_to_send(this->_clients[it->first].get_to_send() + ":" + CLIENT_SOURCE + " PRIVMSG " + channel_message[0] + " " + channel_message[1] + "\r\n");
                this->_clients[it->first].add_epollout(this->_epoll_fd);
                return;
            }
        }
        APPEND_CLIENT_TO_SEND(ERR_NOSUCHNICK());
    }
    else
    {
        map<int, string> m = this->_channels[channel_message[0]].get_users();
        for (map<int, string>::iterator it = m.begin(); it != m.end(); it++)
        {
            if (clientFd != it->first)
            {
                this->_clients[it->first].set_to_send(this->_clients[it->first].get_to_send() + ":" + CLIENT_SOURCE + " PRIVMSG " + channel_message[0] + " " + channel_message[1] + "\r\n");
                this->_clients[it->first].add_epollout(this->_epoll_fd);
            }
        }
    }
}