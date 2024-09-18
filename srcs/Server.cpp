#include "../Includes/Server.hpp"

bool g_sigint = false;

void sigint_handler(int signal)
{
    g_sigint = true;
    cout << RED << "SIGINT detected " << RESET << endl;
    (void)signal;
}

Server::Server() : _servSocket(0), _epoll_fd(0), _cmdMap(create_map())
{
    return;
}

map<string, CommandFunction> const Server::create_map()
{
    map<string, CommandFunction> map;
    map["CAP"] = &Server::CAP;
    map["NICK"] = &Server::NICK;
    map["PASS"] = &Server::PASS;
    map["PING"] = &Server::PING;
    map["USER"] = &Server::USER;
    map["QUIT"] = &Server::QUIT;
    return map;
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
    for (map<int, Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        cout << RED << "Closing socket : " << it->first << RESET << endl;
        close(it->first);
    }
    if (this->_epoll_fd)
        close(this->_epoll_fd);
    if (this->_servSocket)
        close(this->_servSocket);
    return;
}

bool set_socket_non_blocking(int const &fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
    {
        cerr << "fcntl" << endl;
        return FAILURE;
    }

    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        cerr << "fcntl" << endl;
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

    string ip = inet_ntoa(clientAddress.sin_addr);

    set_socket_non_blocking(clientSocket);

    epoll_event clientEvent;
    clientEvent.data.fd = clientSocket;
    clientEvent.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, clientSocket, &clientEvent);

    cout << GREEN << "New client detected : " + ip + " on socket : " << clientSocket << RESET << endl;
    this->_clients[clientSocket] = Client(clientSocket, ip, clientEvent);
}

void Server::disconnect_client(int const &clientFd)
{
    cout << RED << "Client disconnected on socket : " << clientFd << RESET << endl;
    epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, clientFd, NULL);
    close(clientFd);
    this->_clients.erase(clientFd);
}

void Server::receive_message(int const &clientFd)
{
    char buff[BUFFER_SIZE] = {0};

    int bytesRead = read(clientFd, buff, BUFFER_SIZE);

    if (bytesRead == 0)
    {
        disconnect_client(clientFd);
    }
    else
    {
        CLIENT.set_buffer(CLIENT.get_buffer() + buff);
    }

    for (int i = 0; i < bytesRead; i++)
    {
        CLIENT.push_back_charBuffer(buff[i]);
    }
    CLIENT.assign_buffer();

    CLIENT.add_epollout(this->_epoll_fd);

    // cout << YELLOW << "Message from client number " << clientFd << " of len : " << bytesRead << endl;
    // write(1, buff, bytesRead);
    // cout << RESET << endl;
}

void Server::send_message(int const &clientFd)
{

    if (CLIENT.get_buffer() == "\r\n")
        return;

    if (CLIENT.get_to_send().empty() == true)
    {
        this->handle_request(clientFd);
        cout << "Sending  :\n"
             << CLIENT.get_to_send() << endl;
    }

    CLIENT.set_buffer("");
    CLIENT.charBuffer_clear();

    int toSend = BUFFER_SIZE < CLIENT.get_to_send().size() ? BUFFER_SIZE : CLIENT.get_to_send().size();

    int bytesSent = write(clientFd, CLIENT.get_to_send().c_str(), toSend);

    CLIENT.erase_to_send(0, bytesSent);

    if (toSend < BUFFER_SIZE)
    {
        CLIENT.remove_epollout(this->_epoll_fd);
    }

    // cout << "Bytes sent : " << bytesSent << endl
    //      << "rest to send : " << CLIENT.get_to_send() << endl;

    if (bytesSent == -1 || bytesSent != toSend)
    {
        cerr << "Can't send all data" << endl;
        disconnect_client(clientFd);
        return;
    }

    if (CLIENT.get_quit() == true)
    {
        disconnect_client(clientFd);
    }
}

bool Server::init_servAdrress(int const &port)
{
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(this->_servSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)))
    {
        cerr << RED << "Address already in use" << RESET << endl;
        return (FAILURE);
    }
    return SUCCESS;
}

void Server::first_connection(int const &clientFd)
{
    vector<string> v = split(CLIENT.get_buffer(), "\r\n");

    // cout << "Vector size is : " << v.size() << endl;

    if (v.size() == 0)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    else
    {
        for (size_t i = 0; i < v.size(); i++)
        {
            vector<string> words;

            if (v[i].compare(0, 4, "PASS") == 0 || v[i].compare(0, 4, "QUIT") == 0 || v[i].compare(0, 4, "PING") == 0)
            {
                words = split_first_word(v[i], " ");
            }
            else
            {
                words = split(v[i], " ");
            }
            if (words.size() == 0)
            {
                APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
                return;
            }
            else if (words[0] == "CAP")
            {
                CAP(clientFd, words);
            }
            else if (words[0] == "PASS")
            {
                PASS(clientFd, words);
            }
            else if (words[0] == "NICK")
            {
                NICK(clientFd, words);
            }
            else if (words[0] == "USER")
            {
                USER(clientFd, words);
            }
            else
            {
                cout << "Words[0] is : " << words[0] << endl;
                APPEND_CLIENT_TO_SEND(ERR_UNKNOWNCOMMAND());
                return;
            }
            if (CLIENT.get_to_send().empty() == false)
            {
                return;
            }
        }
    }
    if (CLIENT.get_is_valid_pass() == true && CLIENT.get_NICK().empty() == false && CLIENT.get_USER().empty() == false)
    {
        CLIENT.set_is_identified(true);
        APPEND_CLIENT_TO_SEND(RPL_WELCOME() + RPL_YOURHOST() + RPL_CREATED() + RPL_MYINFO() + RPL_MOTDSTART() + RPL_MOTD() + RPL_ENDOFMOTD());
    }
    return;
}

void Server::normal_request(int const &clientFd)
{
    vector<string> v = split(CLIENT.get_buffer(), "\r\n");

    // cout << "Vector size is : " << v.size() << endl;

    if (v.size() == 0)
    {
        APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
        return;
    }
    else
    {
        for (size_t i = 0; i < v.size(); i++)
        {
            vector<string> words;

            if (v[i].compare(0, 4, "PASS") == 0 || v[i].compare(0, 4, "QUIT") == 0 || v[i].compare(0, 4, "PING") == 0)
            {
                words = split_first_word(v[i], " ");
            }
            else
            {
                words = split(v[i], " ");
            }
            if (words.size() == 0)
            {
                APPEND_CLIENT_TO_SEND(ERR_NEEDMOREPARAMS());
                return;
            }
            else
            {
                cout << "Authentified : Words[0] is : " << words[0] << endl;
                map<string, CommandFunction>::iterator it = this->_cmdMap.find(words[0]);
                if (it == this->_cmdMap.end())
                {
                    APPEND_CLIENT_TO_SEND(ERR_UNKNOWNCOMMAND());
                }
                else
                {
                    (this->*(it->second))(clientFd, words);
                }
            }
        }
    }
}

void Server::handle_request(int const &clientFd)
{
    if (CLIENT.get_is_identified() == false)
    {
        first_connection(clientFd);
    }
    else
    {
        normal_request(clientFd);
    }
    return;
}

void Server::loop_server(vector<epoll_event> events)
{
    cout << "Waiting on connection ..." << endl;

    while (g_sigint == false)
    {
        int n = epoll_wait(this->_epoll_fd, events.data(), events.size(), 1000);

        for (int i = 0; i < n; ++i)
        {
            if (events[i].events & EPOLLRDHUP)
            {
                cout << RED << "EPOLLRDHUP" << RESET << endl;
                disconnect_client(CLIENT_ID);
                cout << "Waiting on connection ..." << endl;
            }
            else
            {
                if (events[i].events & EPOLLIN)
                {
                    cout << GREEN << "EPOLLIN" << RESET << endl;

                    if (CLIENT_ID == this->_servSocket)
                    {
                        accept_new_client();
                    }
                    else
                    {
                        receive_message(CLIENT_ID);
                    }
                    cout << "Waiting on connection ..." << endl;
                }
                if (events[i].events & EPOLLOUT)
                {

                    cout << CYAN << "EPOLLOUT" << RESET << endl;

                    if (ends_with(this->_clients[CLIENT_ID].get_buffer(), "\r\n") == SUCCESS || this->_clients[CLIENT_ID].get_to_send().empty() == false)
                    {
                        cout << YELLOW << "Message is finished :" << endl;
                        cout << this->_clients[CLIENT_ID].get_buffer() << RESET << endl;

                        send_message(CLIENT_ID);

                        cout << "Waiting on connection ..." << endl;
                    }
                    // else
                    // {
                    // cout << "Message not finished" << endl;
                    // }
                }
            }
        }
    }
}

bool Server::launch_server(int const &port, char const *password)
{
    this->_servSocket = socket(AF_INET, SOCK_STREAM, 0);

    cout << GREEN << "Launching IRC server on port " << port << " on socket : " << this->_servSocket << RESET << endl;

    set_socket_non_blocking(this->_servSocket);

    int opt = 1;
    setsockopt(this->_servSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (init_servAdrress(port))
    {
        return FAILURE;
    }

    listen(this->_servSocket, 5);

    this->_epoll_fd = epoll_create1(0);
    if (this->_epoll_fd == -1)
    {
        cerr << "epoll_create" << endl;
        return FAILURE;
    }

    epoll_event event;

    event.data.fd = this->_servSocket;
    event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;

    if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_servSocket, &event) == -1)
    {
        cerr << "epoll_ctl" << endl;
        return FAILURE;
    }

    vector<epoll_event> events(500);
    this->_password = password;

    loop_server(events);
    return 0;
}

bool check_port(char *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        if (isdigit(str[i]) == 0)
            return FAILURE;
    }

    int res = atoi(str);

    if (res < 6660 || res > 6669)
        return FAILURE;

    return SUCCESS;
}

bool check_pwd(char *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        if (isprint(str[i]) == 0 || str[i] == ' ')
            return FAILURE;
    }
    return SUCCESS;
}

bool Server::init_server(int ac, char **av)
{
    const string usage = "Usage ./ircserv <port> <password>";
    if (ac != 3)
    {
        cerr << usage << endl;
        return FAILURE;
    }
    if (check_port(av[1]) == FAILURE)
    {
        cerr << usage << endl
             << "Port must be between 6660 and 6669" << endl;
        return FAILURE;
    }
    if (check_pwd(av[2]) == FAILURE)
    {
        cerr << usage << endl
             << "Invalid password, unrecognized characters" << endl;
        return FAILURE;
    }

    signal(SIGINT, sigint_handler);
    return SUCCESS;
}

bool Server::start(int ac, char **av)
{
    if (init_server(ac, av) == FAILURE)
        return FAILURE;

    launch_server(atoi(av[1]), av[2]);

    return SUCCESS;
}