#pragma once

#include "../Includes/ft_irc.hpp"

class Channels
{

private:
    string _topic;
    string password;
    map<int, string> _users;

public:
    Channels();
    Channels(Channels const &to_cpy);
    Channels &operator=(Channels const &cpy);
    ~Channels();

    void set_topic(string const &topic);
    string get_topic() const;

    void add_users(int const &fd, string const &name);
    void remove_users(int const &user);
    map<int, string> &get_users();
    string const append_all_users() const;
};