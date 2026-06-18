
NAME = RPN

SRC = main.cpp \
		RPN.cpp 
		
OBJ = $(SRC:.cpp=.o)

CXX = c++

CXXFLAGS = -Wextra -Wall -Werror -std=c++98 -fsanitize=address -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
