#pragma once

#include "../Includes/ft_irc.hpp"

typedef map<int, pair<string, bool> > mapPair;

class Channels
{

private:
    string _topic;
    time_t _creationTime;
    string _password;
    string _mode;
    map<int, pair<string, bool> > _users; // second.first pour name et second.second pour operator

public:
    Channels();
    Channels(int const &clientFd, string const &clientNick);
    Channels(Channels const &to_cpy);
    Channels &operator=(Channels const &cpy);
    ~Channels();

    void set_topic(string const &topic);
    string get_topic() const;

    time_t const &get_creationTime();

    string const &get_mode() const;
    bool const &mode_contains(char const &c) const;
    void add_mode(char const &newMode);
    void remove_mode(string const &modeToRemove);

    void add_users(int const &fd, string const &name);
    void remove_users(int const &user);
    mapPair const &get_users();
    string const append_all_users() const;
};