#include "Includes/ft_irc.hpp"
#include "Includes/Server.hpp"

int main(int ac, char **av)
{
    Server ircserv;

    ircserv.start(ac, av);
}
