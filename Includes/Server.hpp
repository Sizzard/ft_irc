#pragma once

#include "ft_irc.hpp"
#include "Client.hpp"

class Server
{
private:
    int _servSocket;
    int _epoll_fd;
    std::string _password;
    std::map<int, Client> _clients;

    bool init_server(int ac, char **av);
    bool launch_server(int const &port, char const *password);
    void init_servAdrress(int const &port);
    void accept_new_client();
    void receive_message(int const &clientFd);
    void send_message(int const &clientFd);

    void close_server();

public:
    Server();
    Server(Server const &to_cpy);
    Server &operator=(Server const &cpy);
    ~Server();

    bool start(int ac, char **av);
};