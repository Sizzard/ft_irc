#include "Includes/ft_irc.hpp"
#include "Includes/Client.hpp"

bool g_sigint = false;

void sigint_handler(int signal)
{
    g_sigint = true;
    std::cout << red << "SIGINT detected " << reset << std::endl;
    (void)signal;
}

bool set_socket_non_blocking(int const &fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
    {
        perror("fcntl");
        return FAILURE;
    }

    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        perror("fcntl");
        return FAILURE;
    }

    return SUCCESS;
}

void accept_new_client(int const &servSocket, std::map<int, Client> &clients)
{
    int clientSocket = accept(servSocket, nullptr, nullptr);

    std::cout << green << "New client detected on socket : " << clientSocket << reset << std::endl;

    clients[clientSocket] = Client(clientSocket);
    set_socket_non_blocking(clientSocket);
}

void receive_message(std::map<int, Client> clients, int const &clientFd)
{
    char buff[1024] = {0};

    read(clientFd, buff, sizeof(buff));

    clients[clientFd]._message = buff;

    std::cout << "Message from client number " << clientFd << ": " << buff << std::endl;
}

void init_servAdrress(int const &servSocket, int const &port)
{
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(servSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
}

int server(int const &port)
{
    int servSocket = socket(AF_INET, SOCK_STREAM, 0);

    init_servAdrress(servSocket, port);

    listen(servSocket, 5);

    set_socket_non_blocking(servSocket);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create");
        return FAILURE;
    }

    epoll_event event;

    event.data.fd = servSocket;
    event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, servSocket, &event) == -1)
    {
        perror("epoll_ctl");
        return FAILURE;
    }

    std::vector<epoll_event> events(500);
    std::map<int, Client> clients;

    while (g_sigint == false)
    {
        std::cout << "Waiting on connection ..." << std::endl;

        int n = epoll_wait(epoll_fd, events.data(), events.size(), -1);

        for (int i = 0; i < n; i++)
        {
            if (events[i].events & EPOLLRDHUP)
            {
                std::cout << red << "EPOLLRDHUP\nClosing socket : " << CLIENT_ID << reset << std::endl;
                close(CLIENT_ID);
                clients.erase(CLIENT_ID);
            }
            if (events[i].events & EPOLLIN)
            {
                std::cout << green << "EPOLLIN" << reset << std::endl;

                if (CLIENT_ID == servSocket)
                {
                    accept_new_client(servSocket, clients);
                }
                else
                {
                    receive_message(clients, CLIENT_ID);
                }
            }
            if (events[i].events & EPOLLOUT)
            {
                std::cout << cyan << "EPOLLOUT" << reset << std::endl;
                ;
            }
        }
    }
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        std::cout << red << "Closing socket : " << it->first << reset << std::endl;
        close(it->first);
    }
    close(epoll_fd);
    close(servSocket);
    return 0;
}

bool check_port(char *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        if (std::isdigit(str[i]) == 0)
            return FAILURE;
    }

    int res = std::atoi(str);

    if (res < 6660 || res > 6669)
        return FAILURE;

    return SUCCESS;
}

bool check_pwd(char *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        if (std::isprint(str[i]) == 0 || str[i] == ' ')
            return FAILURE;
    }
    return SUCCESS;
}

bool init_main(int ac, char **av)
{
    const std::string usage = "Usage ./ircserv <port> <password>";
    if (ac != 3)
    {
        std::cerr << usage << std::endl;
        return FAILURE;
    }
    if (check_port(av[1]) == FAILURE)
    {
        std::cerr << usage << std::endl
                  << "Port must be between 6660 and 6669" << std::endl;
        return FAILURE;
    }
    if (check_pwd(av[2]) == FAILURE)
    {
        std::cerr << usage << std::endl
                  << "Invalid password, unrecognized characters" << std::endl;
        return FAILURE;
    }

    std::signal(SIGINT, sigint_handler);
    return SUCCESS;
}

int main(int ac, char **av)
{
    if (init_main(ac, av) == FAILURE)
        return FAILURE;
    return server(std::atoi(av[1]));
}
