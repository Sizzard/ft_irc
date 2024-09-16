#pragma once

#include "ft_irc.hpp"

class Client;

typedef void (Client::*CommandFunction)(const vector<string> &, const map<int, Client> &);

class Client
{
private:
    int _fd;
    string _ip;
    bool _isValidPass;
    bool _isIdentified;

    map<string, CommandFunction> _cmdMap;

    string _buffer;
    string _NICK;
    string _USER;
    string _toSend;

    void first_connection(string const &password, map<int, Client> const &serv);
    void capability_negotiation(vector<string> const &words);
    void verify_password(vector<string> const &words, string const &password);
    void verify_nick(vector<string> const &words, map<int, Client> const &serv);
    void verify_user(vector<string> const &words);

    void normal_request(const map<int, Client> &serv);

    map<string, CommandFunction> const create_map();
    map<string, CommandFunction> const &get_command_map();

    void ping(vector<string> const &words, map<int, Client> const &serv);
    void nick(vector<string> const &words, map<int, Client> const &serv);

public:
    Client();
    Client(int fd, string ip);
    Client(Client const &to_cpy);
    Client &operator=(Client const &cpy);
    ~Client();

    void set_buffer(string newBuffer);
    string const &get_buffer();

    void set_toSend(string const &newToSend);
    string const &get_toSend();

    string const &get_USER();

    bool handle_request(string const &password, map<int, Client> const &serv);
};
