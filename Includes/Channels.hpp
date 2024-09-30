#pragma once

#include "../Includes/ft_irc.hpp"

typedef map<string, pair<int, bool> > mapPair;

class Channels
{

private:
    string _topic;
    time_t _creationTime;
    string _password;
    string _mode;
    int _limit;
    map<string, pair<int, bool> > _users; // second.first pour name et second.second pour operator

public:
    Channels();
    Channels(int const &clientFd, string const &clientNick);
    Channels(Channels const &to_cpy);
    Channels &operator=(Channels const &cpy);
    ~Channels();

    void set_topic(string const &topic);
    string get_topic() const;

    int const &get_limit();
    void set_limit(string const &limit);

    void set_password(string const &password);
    string const &get_password();

    time_t const &get_creationTime();

    string const &get_mode() const;
    bool mode_contains(char const &c) const;
    void add_mode(char const &newMode);
    void remove_mode(char const &modeToRemove);

    void add_users(int const &fd, string const &name);
    void remove_users(string const &clientNick);

    mapPair const &get_users();
    string const append_all_users() const;

    void add_operator(string const &clientNick);
    void remove_operator(string const &clientNick);
};