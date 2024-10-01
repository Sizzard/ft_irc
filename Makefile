CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
NAME = ircserv
SRC = srcs/Server.cpp srcs/Commands.cpp srcs/Client.cpp srcs/Channels.cpp srcs/utils.cpp\
	main.cpp
OBJ = $(SRC:.cpp=.o)

all : $(NAME)

bonus : bonus.o
	c++ $(CPPFLAGS) -o bonus bonus.o

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o:%.cpp Includes/ft_irc.hpp Includes/Server.hpp Includes/Client.hpp Includes/Channels.hpp
	c++ $(CPPFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJ) bonus.o

fclean :
	rm -f $(OBJ) bonus.o
	rm -f $(NAME) bonus

re : fclean all

.PHONY : clean fclean all re