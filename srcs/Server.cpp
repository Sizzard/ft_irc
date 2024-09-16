#include "../Includes/Server.hpp"

bool g_sigint = false;

void sigint_handler(int signal)
{
    g_sigint = true;
    std::cout << RED << "SIGINT detected " << RESET << std::endl;
    (void)signal;
}

Server::Server()
{
    return;
}

Server::Server(Server const &cpy)
{
    *this = cpy;
    return;
}

Server &Server::operator=(Server const &cpy)
{
    this->_servSocket = cpy._servSocket;
    this->_epoll_fd = cpy._epoll_fd;
    this->_clients = cpy._clients;
    return *this;
}

Server::~Server()
{
    return;
}

bool set_socket_non_blocking(int const &fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
    {
        std::cerr << "fcntl" << std::endl;
        return FAILURE;
    }

    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        std::cerr << "fcntl" << std::endl;
        return FAILURE;
    }

    return SUCCESS;
}

void Server::accept_new_client()
{

    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    socklen_t len = sizeof(clientAddress);

    int clientSocket = accept(this->_servSocket, (sockaddr *)&clientAddress, &len);

    std::string ip = inet_ntoa(clientAddress.sin_addr);

    std::cout << GREEN << "New client detected : " + ip + " on socket : " << clientSocket << RESET << std::endl;
    this->_clients[clientSocket] = Client(clientSocket, ip);

    set_socket_non_blocking(clientSocket);

    epoll_event clientEvent;
    clientEvent.data.fd = clientSocket;
    clientEvent.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
    epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, clientSocket, &clientEvent);
}

void Server::disconnect_client(int const &clientFd)
{
    std::cout << RED << "Client disconnected" << RESET << std::endl;
    epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, clientFd, NULL);
    close(clientFd);
    this->_clients.erase(clientFd);
}

void Server::receive_message(int const &clientFd)
{
    char buff[1024] = {0};

    int bytesRead = read(clientFd, buff, sizeof(buff));

    if (bytesRead == 0)
    {
        disconnect_client(clientFd);
    }
    else
    {
        this->_clients[clientFd].set_buffer(this->_clients[clientFd].get_buffer() + buff);
    }

    // std::cout << YELLOW << "Message from client number " << clientFd << std::endl
    //           << buff << RESET << std::endl;
}

void Server::send_message(int const &clientFd)
{
    if (this->_clients[clientFd].get_buffer() == "\r\n")
        return;
    if (this->_clients[clientFd].handle_request(this->_password, this->_clients) == SUCCESS)
    {
        std::cout << "Sending :\n"
                  << this->_clients[clientFd].get_toSend() << std::endl;
        write(clientFd, this->_clients[clientFd].get_toSend().c_str(), this->_clients[clientFd].get_toSend().size());
        this->_clients[clientFd].set_toSend("");
    }
}

void Server::init_servAdrress(int const &port)
{
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(this->_servSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
}

bool Server::launch_server(int const &port, char const *password)
{
    this->_servSocket = socket(AF_INET, SOCK_STREAM, 0);

    std::cout << GREEN << "Launching IRC server on port " << port << " on socket : " << this->_servSocket << RESET << std::endl;

    set_socket_non_blocking(this->_servSocket);

    int opt = 1;
    setsockopt(this->_servSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    init_servAdrress(port);

    listen(this->_servSocket, 5);

    this->_epoll_fd = epoll_create1(0);
    if (this->_epoll_fd == -1)
    {
        std::cerr << "epoll_create" << std::endl;
        return FAILURE;
    }

    epoll_event event;

    event.data.fd = this->_servSocket;
    event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;

    if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_servSocket, &event) == -1)
    {
        std::cerr << "epoll_ctl" << std::endl;
        return FAILURE;
    }

    std::vector<epoll_event> events(500);
    this->_password = password;

    while (g_sigint == false)
    {
        std::cout << "Waiting on connection ..." << std::endl;

        int n = epoll_wait(this->_epoll_fd, events.data(), events.size(), -1);

        for (int i = 0; i < n; ++i)
        {
            if (events[i].events & EPOLLRDHUP)
            {
                std::cout << RED << "EPOLLRDHUP\nClosing socket : " << CLIENT_ID << RESET << std::endl;
                disconnect_client(CLIENT_ID);
            }
            else
            {
                if (events[i].events & EPOLLIN)
                {
                    std::cout << GREEN << "EPOLLIN" << RESET << std::endl;

                    if (CLIENT_ID == this->_servSocket)
                    {
                        accept_new_client();
                    }
                    else
                    {
                        receive_message(CLIENT_ID);
                    }
                }
                if (events[i].events & EPOLLOUT)
                {
                    std::cout << CYAN << "EPOLLOUT" << RESET << std::endl;

                    if (ends_with(this->_clients[CLIENT_ID].get_buffer(), "\r\n") == SUCCESS)
                    {
                        std::cout << YELLOW << "Message is finished :" << std::endl;
                        std::cout << this->_clients[CLIENT_ID].get_buffer() << RESET << std::endl;

                        send_message(CLIENT_ID);

                        this->_clients[CLIENT_ID].set_buffer("");
                    }
                    else
                    {
                        std::cout << "Message not finished" << std::endl;
                    }
                }
            }
        }
    }
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

bool Server::init_server(int ac, char **av)
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

void Server::close_server()
{
    for (std::map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        std::cout << RED << "Closing socket : " << it->first << RESET << std::endl;
        close(it->first);
    }
    close(this->_epoll_fd);
    close(this->_servSocket);
}

bool Server::start(int ac, char **av)
{
    if (init_server(ac, av) == FAILURE)
        return FAILURE;

    launch_server(std::atoi(av[1]), av[2]);
    close_server();

    return SUCCESS;
}