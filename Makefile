CPPFLAGS = -Wall -Wextra -Werror -g3
NAME = ircserv
SRC = Client.cpp\
	main.cpp
OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o:%.cpp 
	c++ $(CPPFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJ)

fclean :
	rm -f $(OBJ)
	rm -f $(NAME)

re : fclean all

.PHONY : clean fclean all re