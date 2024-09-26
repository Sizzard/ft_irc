#pragma once

#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channels.hpp"

class Server;

typedef void (Server::*CommandFunction)(int const &clientFd, const vector<string> &);

class Server
{
private:
    int _servSocket;
    int _epoll_fd;
    string _password;
    string _creationTime;
    map<int, Client> _clients;
    map<string, Channels> _channels;

    map<string, CommandFunction> _cmdMap;

    map<string, CommandFunction> const create_map();

    bool init_server(int ac, char **av);
    bool launch_server(int const &port, char const *password);
    bool init_servAdrress(int const &port);
    void loop_server(vector<epoll_event> events);

    void accept_new_client();
    void disconnect_client(int const &clientFd);

    void receive_message(int const &clientFd);
    void send_message(int const &clientFd);

    void handle_request(int const &clientFd);

    void first_connection(int const &clientFd);
    void normal_request(int const &clientFd);

    void send_to_all_clients_in_chan(string const &channelName, string const &message);
    void send_to_all_clients_in_chan_except(int const &clientFd, string const &channelName, string const &message);

    void quit_all_channels(int const &clientFd);

    void CAP(int const &clientFd, vector<string> const &words);
    void NICK(int const &clientFd, vector<string> const &words);
    void PASS(int const &clientFd, vector<string> const &words);
    void PING(int const &clientFd, vector<string> const &words);
    void USER(int const &clientFd, vector<string> const &words);
    void QUIT(int const &clientFd, vector<string> const &words);
    void JOIN(int const &clientFd, vector<string> const &words);
    void PRIVMSG(int const &clientFd, vector<string> const &words);
    void TOPIC(int const &clientFd, vector<string> const &words);
    void MODE(int const &clientFd, vector<string> const &words);
    void CHANNELS(int const &clientFd, vector<string> const &words);

    bool handle_mode_cases(int const &clientFd, vector<string> const &words);
    void handle_i(int const &clientFd, vector<string> const &words, map<char, char>::const_iterator const &it);
    void handle_t(int const &clientFd, vector<string> const &words, map<char, char>::const_iterator const &it);
    void handle_k(int const &clientFd, vector<string> const &words, map<char, char>::const_iterator const &it, vector<string>::const_iterator const &args);
    void handle_o(int const &clientFd, vector<string> const &words, map<char, char>::const_iterator const &it, vector<string>::const_iterator const &args);

public:
    Server();
    Server(Server const &to_cpy);
    Server &operator=(Server const &cpy);
    ~Server();

    bool start(int ac, char **av);
};

#define APPEND_CLIENT_TO_SEND(buff) CLIENT.append_to_send(buff)