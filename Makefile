CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
NAME = ircserv
SRC = srcs/Server.cpp srcs/Commands.cpp srcs/Client.cpp srcs/Channels.cpp srcs/utils.cpp\
	main.cpp
OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o:%.cpp Includes/ft_irc.hpp Includes/Server.hpp Includes/Client.hpp Includes/Channels.hpp
	c++ $(CPPFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJ)

fclean :
	rm -f $(OBJ)
	rm -f $(NAME)

re : fclean all

.PHONY : clean fclean all re