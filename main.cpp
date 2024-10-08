#include "Includes/ft_irc.hpp"
#include "Includes/Server.hpp"

int main(int ac, char **av)
{
    Server ircserv;

    try
    {
        ircserv.start(ac, av);
    }
    catch (std::exception const &e)
    {
        std::cerr << RED << e.what() << RESET << std::endl;
    }
}
