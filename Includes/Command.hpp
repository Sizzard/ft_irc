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