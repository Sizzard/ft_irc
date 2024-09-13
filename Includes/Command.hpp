#pragma once

#include <iostream>

class Command
{
    private:
    

public:
    Command();
    Command(Command const &to_cpy);
    Command &operator=(Command const &cpy);
    ~Command();
};

enum CommandEnum
{
    CAP = 1,
    PASS = 2,
    NICK = 3,
    USER = 4,
    JOIN = 5,
    PRIVMSG = 6,
    MODE = 7,
    TOPIC = 8,
    LIST = 9,
    NOTIC = 10,
};