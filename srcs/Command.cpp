
#include "../Includes/Command.hpp"

Command::Command()
{
    return;
}

Command::Command(Command const &cpy)
{
    *this = cpy;
    return;
}

Command &Command::operator=(Command const &cpy)
{
    // Copy what you want : this->name = cpy.name
    return *this;
}

Command::~Command()
{
    return;
}