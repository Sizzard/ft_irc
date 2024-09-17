#pragma once

#include "ft_irc.hpp"
#include "Client.hpp"

class Server;

typedef void (Server::*CommandFunction)(int const &clientFd, const vector<string> &);

class Server
{
private:
    int _servSocket;
    int _epoll_fd;
    std::string _password;
    std::map<int, Client> _clients;
    std::map<std::string, CommandFunction> _cmdMap;

    map<string, CommandFunction> const create_map();

    bool init_server(int ac, char **av);
    bool launch_server(int const &port, char const *password);
    void init_servAdrress(int const &port);
    void loop_server(vector<epoll_event> events);
    void accept_new_client();
    void disconnect_client(int const &clientFd);
    void receive_message(int const &clientFd);
    void send_message(int const &clientFd);

    void first_connection(int const &clientFd, string const &password);
    void CAP(int const &clientFd, vector<string> const &words);
    void PASS(int const &clientFd, vector<string> const &words, string const &password);
    void USER(int const &clientFd, vector<string> const &words);

    bool handle_request(int const &clientFd, string const &password);
    void normal_request(int const &clientFd);

    void PING(int const &clientFd, vector<string> const &words);
    void NICK(int const &clientFd, vector<string> const &words);

    void close_server();

public:
    Server();
    Server(Server const &to_cpy);
    Server &operator=(Server const &cpy);
    ~Server();

    bool start(int ac, char **av);
};

#define APPEND_CLIENT_TO_SEND(buff) CLIENT.set_to_send(CLIENT.get_to_send() + buff)