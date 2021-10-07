SRCS = 	main.cpp \
		Server.cpp \
		Client.cpp \
		Commands.cpp \
		ft_errors.cpp \
		ft_reply.cpp \
		ft_print_help.cpp

OBJ = $(SRCS:.cpp=.o)

CXX = clang++

CXXFLAGS = -Wall -Werror -Wextra -g3 -std=c++98 #-fsanitize=address 

NAME = IRC_90s

INCLUDE = headers.hpp \
		  Server.hpp \
		  Client.hpp

#$(subst cpp,hpp, $(SRCS))	

%.o:%.cpp $(INCLUDE)
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -I . $(OBJ) -o $(NAME)

echo :
	@echo $(SRCS)
	@echo $(INCLUDE)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
