
#include "Channels.hpp"

Channels::Channels()
{
    return;
}

Channels::Channels(Channels const &cpy)
{
    *this = cpy;
    return;
}

Channels &Channels::operator=(Channels const &cpy)
{
    (void)cpy;
    return *this;
}

Channels::~Channels()
{
    return;
}