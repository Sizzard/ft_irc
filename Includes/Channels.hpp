#pragma once

#include <iostream>

class Channels
{

public:
    Channels();
    Channels(Channels const &to_cpy);
    Channels &operator=(Channels const &cpy);
    ~Channels();
};