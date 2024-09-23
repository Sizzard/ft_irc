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
    if (words.size() < 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    if (*words[1].begin() == '#' || *words[1].begin() == ':')
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
        APPEND_CLIENT_TO_SEND("+" + CLIENT_SOURCE + " QUIT :\r\n");
    }

    else
    {
        APPEND_CLIENT_TO_SEND(":" + CLIENT_SOURCE + " QUIT " + words[1] + "\r\n");
    }

    for (vector<string>::const_iterator it = CLIENT.get_inChannel().begin(); it != CLIENT.get_inChannel().end(); it++)
    {
        send_to_all_clients_in_chan(clientFd, *it, ":" + CLIENT_SOURCE + " QUIT " + (words.size() == 1 ? "" : words[1]) + "\r\n");
    }

    CLIENT.set_quit(true);
    return;
}

void Server::JOIN(int const &clientFd, vector<string> const &words)
{
    if (words.size() != 2 || words[1].empty() == true)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    vector<string> channelsToJoin = split(words[1], ",");

    for (size_t i = 0; i < channelsToJoin.size(); i++)
    {
        cout << YELLOW << channelsToJoin[i] << RESET << endl;
        if (this->_channels.find(channelsToJoin[i]) == this->_channels.end())
        {
            this->_channels[channelsToJoin[i]] = Channels();
        }
        for (map<int, string>::iterator it = this->_channels[channelsToJoin[i]].get_users().begin(); it != this->_channels[channelsToJoin[i]].get_users().end(); it++)
        {
            Client &client = this->_clients[it->first];
            client.set_to_send(client.get_to_send() + ":" + CLIENT_SOURCE + " JOIN :" + channelsToJoin[i] + "\r\n");
            client.add_epollout(this->_epoll_fd);
        }
        CLIENT.add_to_inChannel(channelsToJoin[i]);
        this->_channels[channelsToJoin[i]].add_users(clientFd, CLIENT.get_NICK());
        APPEND_CLIENT_TO_SEND(":" + CLIENT_SOURCE + " JOIN :" + channelsToJoin[i] + "\r\n");
        if (this->_channels[channelsToJoin[i]].get_topic().empty() != true)
            APPEND_CLIENT_TO_SEND(RPL_TOPIC(channelsToJoin[i]));
        APPEND_CLIENT_TO_SEND(RPL_NAMREPLY() + RPL_ENDOFNAMES());
    }
    return;
}

void Server::PRIVMSG(int const &clientFd, vector<string> const &words)
{
    if (words.size() < 2 || words[1].empty() == true)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        CLIENT.add_epollout(this->_epoll_fd);
        return;
    }

    vector<string> channel_message = split_first_word(words[1], " ");

    if (channel_message.size() < 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        CLIENT.add_epollout(this->_epoll_fd);
        return;
    }

    vector<string> toSend = split(channel_message[0], ",");

    for (vector<string>::iterator ite = toSend.begin(); ite != toSend.end(); ite++)
    {
        map<string, Channels>::iterator it = this->_channels.find(*ite);
        if (it == this->_channels.end())
        {
            bool clientExist = false;
            for (map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
            {
                if (it->second.get_NICK() == *ite)
                {
                    this->_clients[it->first].set_to_send(this->_clients[it->first].get_to_send() + ":" + CLIENT_SOURCE + " PRIVMSG " + *ite + " " + channel_message[1] + "\r\n");
                    this->_clients[it->first].add_epollout(this->_epoll_fd);
                    clientExist = true;
                }
            }
            if (clientExist == false)
            {
                APPEND_CLIENT_TO_SEND(ERR_NOSUCHNICK());
            }
        }
        else
        {
            send_to_all_clients_in_chan(clientFd, *ite, ":" + CLIENT_SOURCE + " PRIVMSG " + *ite + " " + channel_message[1] + "\r\n");
        }
    }
}

void Server::TOPIC(int const &clientFd, vector<string> const &words)
{
    CLIENT.add_epollout(this->_epoll_fd);

    if (words.size() == 1 || words[1].empty() == true)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    vector<string> channel = split_first_word(words[1], " ");

    if (channel.size() == 0)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    vector<string> map = CLIENT.get_inChannel();

    if (channel.size() == 1)
    {
        // cout << "TEST 2: " << channel[0] << endl;

        for (vector<string>::iterator it = map.begin(); it != map.end(); it++)
        {
            // cout << "TEST 3: " << channel[0] << endl;
            if (channel[0] == *it)
            {
                if (this->_channels[*it].get_topic().empty() == true)
                    APPEND_CLIENT_TO_SEND(RPL_NOTOPIC(*it));
                else
                    APPEND_CLIENT_TO_SEND(RPL_TOPIC(*it));
                return;
            }
        }
        APPEND_CLIENT_TO_SEND(ERR_NOTONCHANNEL());
    }
    else
    {
        if (channel[1] == ":")
        {
            this->_channels[channel[0]].set_topic("");
        }
        else
        {
            for (vector<string>::iterator it = map.begin(); it != map.end(); it++)
            {
                channel[1] = channel[1].substr(1);
                if (channel[0] == *it)
                {
                    this->_channels[channel[0]].set_topic(channel[1]);
                    send_to_all_clients_in_chan(clientFd, channel[0], RPL_TOPIC(channel[0]));
                    APPEND_CLIENT_TO_SEND(RPL_TOPIC(channel[0]));
                }
            }
        }
    }
}