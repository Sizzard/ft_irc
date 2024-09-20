#pragma once

#include "ft_irc.hpp"

class Client
{
private:
    int _fd;
    string _ip;
    bool _isValidPass;
    bool _isIdentified;
    bool _quit;
    vector<string> _inChannel;
    epoll_event _event;

    vector<char> _charBuffer;
    string _buffer;
    string _NICK;
    string _USER;
    string _network;
    string _toSend;

public:
    Client();
    Client(int fd, string ip, epoll_event clientEvent);
    Client(Client const &to_cpy);
    Client &operator=(Client const &cpy);
    ~Client();

    void set_ip(int newIp);
    string const &get_ip();

    void set_is_valid_pass(bool newIsValidPass);
    bool const &get_is_valid_pass();

    void set_is_identified(bool newIsIdentified);
    bool const &get_is_identified();

    void set_quit(bool newQuit);
    bool const &get_quit();

    void add_to_inChannel(string newInChannel);
    void remove_to_inChannel(string const &InChannelToRemove);
    vector<string> const &get_inChannel();

    void push_back_charBuffer(char c);
    vector<char> const &get_charBuffer();
    void charBuffer_clear();

    void assign_buffer();
    void set_buffer(string newBuffer);
    string const &get_buffer();

    void erase_to_send(size_t pos, size_t len);
    void set_to_send(string const &newToSend);
    string const &get_to_send();

    void set_NICK(string const &newNICK);
    string const &get_NICK();

    void set_USER(string const &newUSER);
    string const &get_USER();

    void set_network(string const &newNetwork);
    string const &get_network();

    void set_quitMsg(string newQuitMsg);
    string const &get_quitMsg();

    void remove_epollout(int const &epoll_fd);
    void add_epollout(int const &epoll_fd);
    epoll_event &get_epoll_event();
};
