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
    if (CLIENT.get_is_valid_pass() == true)
        return;

    // CLIENT.set_is_valid_pass(false);

    if (words.size() != 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    if (words[0] == "PASS" && words[1] == this->_password)
    {
        cout << "VALID PASS" << endl;
        CLIENT.set_is_valid_pass(true);
        return;
    }

    APPEND_CLIENT_TO_SEND(ERR_PASSWDMISMATCH());
    return;
}

void Server::change_nick_in_channels(int const &clientFd, string const &newNick)
{
    for (vector<string>::const_iterator it = CLIENT.get_channelList().begin(); it != CLIENT.get_channelList().end(); it++)
    {
        CHANNEL(*it).change_nick(CLIENT.get_NICK(), newNick);
        send_to_all_clients_in_chan_except(clientFd, *it, RPL_CHANGE_NICK(newNick));
    }
    APPEND_CLIENT_TO_SEND(RPL_CHANGE_NICK(newNick));
    try
    {
        CLIENT.add_epollout(this->_epoll_fd);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
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

    if (CLIENT.get_channelList().empty() == false)
        change_nick_in_channels(clientFd, words[1]);

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
    for (vector<string>::const_iterator it = CLIENT.get_channelList().begin(); it != CLIENT.get_channelList().end(); it++)
    {
        send_to_all_clients_in_chan(*it, ":" + CLIENT_SOURCE + " QUIT :Quit " + (words.size() == 1 ? "" : words[1]) + "\r\n");
    }

    CLIENT.set_quit(true);
    return;
}

void Server::join_channels(int const &clientFd, string const &channelToJoin, string const &password)
{
    cout << YELLOW << channelToJoin << RESET << endl;

    map<string, Channels>::iterator it = this->_channels.find(channelToJoin);

    if (*channelToJoin.begin() != '#')
    {
        APPEND_CLIENT_TO_SEND(ERR_UNKNOWNERROR("channel name not beginning with #"));
        return;
    }
    bool newChannel = false;
    if (it == this->_channels.end())
    {
        newChannel = true;
        this->_channels[channelToJoin] = Channels(clientFd, CLIENT.get_NICK());
        it = this->_channels.find(channelToJoin);
    }
    else if (it->second.mode_contains('k'))
    {
        if (it->second.get_password() != password)
        {
            APPEND_CLIENT_TO_SEND(ERR_BADCHANNELKEY(channelToJoin));
            return;
        }
    }
    if (static_cast<std::size_t>(it->second.get_limit()) <= it->second.get_users().size())
    {
        APPEND_CLIENT_TO_SEND(ERR_CHANNELISFULL(channelToJoin));
        return;
    }
    if (find(it->second.get_invitedUsers().begin(), it->second.get_invitedUsers().end(), clientFd) == it->second.get_invitedUsers().end() && it->second.mode_contains('i'))
    {
        APPEND_CLIENT_TO_SEND(ERR_INVITEONLYCHAN(channelToJoin));
        return;
    }
    if (!newChannel)
        this->_channels[channelToJoin].add_users(clientFd, CLIENT.get_NICK());
    send_to_all_clients_in_chan_except(clientFd, channelToJoin, ":" + CLIENT_SOURCE + " JOIN :" + channelToJoin + "\r\n");
    CLIENT.add_to_channelList(channelToJoin);
    APPEND_CLIENT_TO_SEND(":" + CLIENT_SOURCE + " JOIN :" + channelToJoin + "\r\n");
    if (this->_channels[channelToJoin].get_topic().empty() != true)
        APPEND_CLIENT_TO_SEND(RPL_TOPIC(channelToJoin));
    APPEND_CLIENT_TO_SEND(RPL_NAMREPLY(channelToJoin) + RPL_ENDOFNAMES(channelToJoin));
}

void Server::JOIN(int const &clientFd, vector<string> const &words)
{
    if ((words.size() != 2 && words.size() != 3) || words[1].empty() == true)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    for (vector<string>::const_iterator it = words.begin(); it != words.end(); it++)
    {
        cout << "WORD : " << *it << endl;
    }
    vector<string> const channelsToJoin = split(words[1], ",");
    vector<string> passwords;
    if (words.size() == 3)
        passwords = split(words[2], ",");
    size_t passwords_number = passwords.size();
    for (size_t i = 0; i < channelsToJoin.size(); i++)
    {
        if (passwords_number < i || passwords_number == 0)
            join_channels(clientFd, channelsToJoin[i], "");
        else
            join_channels(clientFd, channelsToJoin[i], passwords[i]);
    }
    return;
}

void Server::PRIVMSG(int const &clientFd, vector<string> const &words)
{
    std::cout << words.size() << std::endl;
    try
    {
        this->_clients[clientFd].add_epollout(this->_epoll_fd);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << endl;
    }
    if (words.size() < 2 || words[1].empty() == true)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    vector<string> channel_message = split_first_word(words[1], " ");
    if (channel_message.size() < 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
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
                    try
                    {
                        it->second.add_epollout(this->_epoll_fd);
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << e.what() << endl;
                    }
                        this->_clients[it->first].append_to_send(":" + CLIENT_SOURCE + " PRIVMSG " + *ite + " " + channel_message[1] + "\r\n");
                        clientExist = true;
                }
            }
            if (clientExist == false)
            {
                APPEND_CLIENT_TO_SEND(ERR_NOSUCHNICK(channel_message[0]));
            }
        }
        else if(it->second.get_users().find(CLIENT.get_NICK()) == it->second.get_users().end())
        {
            APPEND_CLIENT_TO_SEND(ERR_CANNOTSENDTOCHAN(*ite));
        }
        else
        {
            send_to_all_clients_in_chan_except(clientFd,*ite, ":" + CLIENT_SOURCE + " PRIVMSG " + *ite + " " + channel_message[1] + "\r\n");
        }
    }
}

void Server::TOPIC(int const &clientFd, vector<string> const &words)
{
    try
    {
        CLIENT.add_epollout(this->_epoll_fd);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

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

    vector<string> map = CLIENT.get_channelList();

    if (channel.size() == 1 && this->_channels.find(channel[0]) != this->_channels.end())
    {
        if (this->_channels[channel[0]].get_topic().empty() == true)
            APPEND_CLIENT_TO_SEND(RPL_NOTOPIC(channel[0]));
        else
            APPEND_CLIENT_TO_SEND(RPL_TOPIC(channel[0]));
        return;
    }
    else if (this->_channels.find(channel[0]) == this->_channels.end())
    {
        APPEND_CLIENT_TO_SEND(ERR_NOSUCHCHANNEL(channel[0]));
    }
    else
    {
        if (channel[1].compare(0, 1, ":") == 0)
        {
            if (this->_channels.find(channel[0]) == this->_channels.end())
            {
                APPEND_CLIENT_TO_SEND(ERR_NOTONCHANNEL());
            }
            else
            {
                channel[1].erase(0, 1);
                if (CHANNEL(channel[0]).mode_contains('t') == true)
                {
                    if (CHANNEL(channel[0]).is_operator(CLIENT.get_NICK()) == true)
                    {
                        this->_channels[channel[0]].set_topic(channel[1]);
                        send_to_all_clients_in_chan(channel[0], RPL_TOPIC(channel[0]));
                    }
                    else
                    {
                        APPEND_CLIENT_TO_SEND(ERR_CHANOPRIVSNEEDED());
                    }
                }
                else
                {
                    this->_channels[channel[0]].set_topic(channel[1]);
                    send_to_all_clients_in_chan(channel[0], RPL_TOPIC(channel[0]));
                }
            }
        }
    }
}

void Server::CHANNELS(int const &clientFd, vector<string> const &words)
{
    (void)words;
    (void)clientFd;
    for (map<string, Channels>::iterator chanIt = this->_channels.begin(); chanIt != this->_channels.end(); chanIt++)
    {

        cout << YELLOW << "Channel name : " << chanIt->first << endl
             << "USERS :" << RESET << endl;

        for (mapPair::const_iterator usersIt = chanIt->second.get_users().begin(); usersIt != chanIt->second.get_users().end(); usersIt++)
        {
            cout << MAGENTA << usersIt->first << RESET << endl;
        }
    }
    cout << endl;
}

void Server::handle_i(int const &clientFd, vector<string> const &words, vec_pair::const_iterator const &it)
{
    if (it->second == '+')
    {
        CHANNEL(words[1]).add_mode(it->first);
    }
    else if (it->second == '-')
    {
        CHANNEL(words[1]).remove_mode(it->first);
    }
    else
    {
        return;
    }
    send_to_all_clients_in_chan(words[1], ":" + CLIENT_SOURCE + " MODE " + words[1] + " " + it->second + "i\r\n");
}

void Server::handle_t(int const &clientFd, vector<string> const &words, vec_pair::const_iterator const &it)
{
    if (it->second == '+')
    {
        CHANNEL(words[1]).add_mode(it->first);
    }
    else if (it->second == '-')
    {
        CHANNEL(words[1]).remove_mode(it->first);
    }
    else
    {
        return;
    }
    send_to_all_clients_in_chan(words[1], ":" + CLIENT_SOURCE + " MODE " + words[1] + " " + it->second + "t\r\n");
}

void Server::handle_k(int const &clientFd, vector<string> const &words, vec_pair::const_iterator const &it, vector<string>::const_iterator const &args)
{
    if ((unsigned long)std::distance(words.begin(), args) > words.size() || args == words.end())
        return;

    if (it->second == '+')
    {
        CHANNEL(words[1]).add_mode(it->first);
        CHANNEL(words[1]).set_password(*args);
    }
    else if (it->second == '-')
    {
        CHANNEL(words[1]).remove_mode(it->first);
    }
    else
    {
        return;
    }
    send_to_all_clients_in_chan(words[1], ":" + CLIENT_SOURCE + " MODE " + words[1] + " " + it->second + "k " + CHANNEL(words[1]).get_password() + "\r\n");
}

void Server::handle_o(int const &clientFd, vector<string> const &words, vec_pair::const_iterator const &it, vector<string>::const_iterator const &args)
{
    if ((unsigned long)std::distance(words.begin(), args) > words.size())
        return;

    if (CHANNEL(words[1]).get_users().find(*args) == CHANNEL(words[1]).get_users().end())
    {
        APPEND_CLIENT_TO_SEND(ERR_NOSUCHNICK(words[1]));

        for (map<int, Client>::iterator ite = this->_clients.begin(); ite != this->_clients.end(); ite++)
        {
            if (ite->second.get_NICK() == *args)
                APPEND_CLIENT_TO_SEND(ERR_USERNOTINCHANNEL(words[1], *args));
        }
        return;
    }

    if (it->second == '+')
    {
        CHANNEL(words[1]).add_operator(*args);
    }
    else if (it->second == '-')
    {
        CHANNEL(words[1]).remove_operator(*args);
    }
    else
    {
        return;
    }
    send_to_all_clients_in_chan(words[1], ":" + CLIENT_SOURCE + " MODE " + words[1] + " " + it->second + "o " + *args + "\r\n");
}

void Server::handle_l(int const &clientFd, vector<string> const &words, vec_pair::const_iterator const &it, vector<string>::const_iterator const &args)
{
    if ((unsigned long)std::distance(words.begin(), args) > words.size())
        return;

    if (it->second == '+')
    {
        CHANNEL(words[1]).add_mode(it->first);
        if (args != words.end())
        {
            try
            {
                CHANNEL(words[1]).set_limit(*args);
            }
            catch (const std::exception &e)
            {
                APPEND_CLIENT_TO_SEND(ERR_INVALIDMODEPARAM(words[1], *args));
                return;
            }
        }
        else
        {
            APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
            return;
        }
    }
    else if (it->second == '-')
    {
        CHANNEL(words[1]).remove_mode(it->first);
        CHANNEL(words[1]).set_limit("100");
    }
    else
    {
        return;
    }
    cout << CHANNEL(words[1]).get_limit() << endl;
    send_to_all_clients_in_chan(words[1], ":" + CLIENT_SOURCE + " MODE " + words[1] + " " + it->second + "l " + (args == words.end() ? "" : *args) + "\r\n");
}

bool Server::handle_mode_cases(int const &clientFd, vector<string> const &words)
{
    string str(words[2]);

    vec_pair m = split_mode(str);

    for (vec_pair::iterator it = m.begin(); it != m.end(); it++)
        cout << it->first << " : " << it->second << endl;

    vector<string>::const_iterator args = words.begin() + 3;

    for (vec_pair::const_iterator it = m.begin(); it != m.end(); it++)
    {
        switch (it->first)
        {
        case 'i':
            handle_i(clientFd, words, it);
            break;
        case 't':
            handle_t(clientFd, words, it);
            break;
        case 'k':
            handle_k(clientFd, words, it, args++);
            break;
        case 'o':
            handle_o(clientFd, words, it, args++);
            break;
        case 'l':
            handle_l(clientFd, words, it, args++);
            break;
        }
    }
    cout << CHANNEL(words[1]).get_mode() << " " << CHANNEL(words[1]).get_password() << endl;
    return SUCCESS;
}

void Server::MODE(int const &clientFd, vector<string> const &words)
{
    try
    {
        CLIENT.add_epollout(this->_epoll_fd);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    if (words.size() < 2)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }

    map<string, Channels>::iterator it = this->_channels.find(words[1]);

    if (it != this->_channels.end())
    {
        if (words.size() < 3)
        {
            APPEND_CLIENT_TO_SEND(RPL_CHANNELMODEIS(words[1]) + RPL_CREATIONTIME(words[1]));
        }
        else
        {

            if (it->second.is_operator(CLIENT.get_NICK()) == false)
            {
                APPEND_CLIENT_TO_SEND(ERR_CHANOPRIVSNEEDED());
            }
            else
            {
                handle_mode_cases(clientFd, words);
            }
        }
    }
    else if (words[1] == CLIENT.get_NICK())
    {
        APPEND_CLIENT_TO_SEND(RPL_UMODEIS());
    }
    else
    {
        bool flag = false;
        for (map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
        {
            if (it->second.get_NICK() == words[1])
            {
                APPEND_CLIENT_TO_SEND(ERR_USERSDONTMATCH());
                flag = true;
            }
        }
        if (flag == false)
        {
            APPEND_CLIENT_TO_SEND(ERR_NOSUCHNICK(words[1]));
        }
    }
}

void Server::KICK(int const &clientFd, vector<string> const &words)
{
    if (words.size() < 3)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    map<string, Channels>::iterator it = this->_channels.find(words[1]);
    if (it != this->_channels.end())
    {
        mapPair user_list = it->second.get_users();
        mapPair::iterator user_requesting_kick = user_list.find(CLIENT.get_NICK());
        if (user_requesting_kick != user_list.end())
        {
            if (user_requesting_kick->second.second == false)
            {
                APPEND_CLIENT_TO_SEND(ERR_CHANOPRIVSNEEDED());
                return;
            }
            std::vector<std::string> users_to_kick = split(words[2], ",");
            while (!users_to_kick.empty())
            {
                mapPair::iterator user_to_kick = user_list.find(users_to_kick.front());
                if (user_to_kick != user_list.end())
                {
                    std::string default_msg = ":" + CLIENT.get_NICK();
                    if (words.size() == 4 && words[3].compare(":") != 0)
                        default_msg = words[3];
                    send_to_all_clients_in_chan(words[1], ":" + CLIENT_SOURCE + " KICK " + words[1] + " " + users_to_kick.front() + " " + default_msg + "\r\n");
                    it->second.remove_users(words[2]);
                    CLIENT.remove_from_channelList(words[1]);
                    if (CHANNEL(words[1]).get_users().empty() == true)
                    {
                        cout << RED << "Nobody in channel " << words[1] << " anymore, deleting it" << RESET << endl;
                        this->_channels.erase(words[1]);
                    }
                }
                else
                    APPEND_CLIENT_TO_SEND(ERR_USERNOTINCHANNEL(words[1], users_to_kick.front()));
                users_to_kick.erase(users_to_kick.begin());
            }
        }
        else
            APPEND_CLIENT_TO_SEND(ERR_NOTONCHANNEL());
    }
    else
        APPEND_CLIENT_TO_SEND(ERR_NOSUCHCHANNEL(words[1]));
}

void Server::INVITE(int const &clientFd, vector<string> const &words)
{
    if (words.size() < 3)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    map<string, Channels>::iterator it = this->_channels.find(words[2]);
    if (it != this->_channels.end())
    {
        mapPair user_list = it->second.get_users();
        mapPair::iterator user_inviting = user_list.find(CLIENT.get_NICK());
        if (user_inviting != user_list.end())
        {

            if (it->second.mode_contains('i') && user_inviting->second.second == false)
            {
                std::cout << user_inviting->second.second << std::endl;
                std::cout << user_inviting->first << std::endl;
                APPEND_CLIENT_TO_SEND(ERR_CHANOPRIVSNEEDED());
                return;
            }
            mapPair::iterator user_to_invite = user_list.find(words[1]);
            if (user_to_invite == user_list.end())
            {
                int const fd = find_client_fd(words[1]);
                if (fd == -1)
                {
                    APPEND_CLIENT_TO_SEND(ERR_NOSUCHNICK(words[1]));
                    return;
                }
                APPEND_CLIENT_TO_SEND(RPL_INVITING(words[1], words[2]));
                it->second.invite_user(fd);
                try
                {
                    this->_clients[fd].add_epollout(this->_epoll_fd);
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << endl;
                }
                APPEND_USER_TO_SEND(fd, ":" + CLIENT_SOURCE + CLIENT.get_NICK() + " INVITE " + words[1] + " " + words[2] + "\r\n");
            }
            else
                APPEND_CLIENT_TO_SEND(ERR_USERONCHANNEL(words[1]));
        }
        else
            APPEND_CLIENT_TO_SEND(ERR_NOTONCHANNEL());
    }
    else
        APPEND_CLIENT_TO_SEND(ERR_NOSUCHCHANNEL(words[1]));
}
