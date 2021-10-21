NAME = IRC_90s

SRCS = 	main.cpp \
		Server.cpp \
		Client.cpp \
		Commands.cpp \
		Channel.cpp \
		ft_errors.cpp \
		ft_reply.cpp \
		ft_print_help.cpp

# OBJ = $(SRCS:.cpp=.o)
OBJ_DIR   = objs/

OBJS      = $(addprefix $(OBJ_DIR), ${SRCS:.cpp=.o})

DIR		  = $(sort $(dir $(OBJS)))

CXX = clang++

CXXFLAGS = -Wall -Werror -Wextra -g3 -std=c++98 -fsanitize=address 

RM        = rm -rf

INCLUDE = headers.hpp \
		  Server.hpp \
		  Client.hpp \
		  Commands.hpp

MAKE	+= --no-print-directory

# %.o:%.cpp $(INCLUDE)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

all: 
	@${MAKE} -j ${NAME}

$(NAME) :   ${OBJS}	
			@${CXX} -o $@ ${OBJS} ${CXXFLAGS}
			@printf "\033[32mAll done.\n\033[0m\n"

$(DIR) :    
	@mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(INCLUDE) | $(DIR)
		$(CXX) -c $< -o $@ ${CXXFLAGS}

echo :
	@echo $(SRCS)
	@echo $(INCLUDE)


clean:
	@echo "Cleaning files..."
	@${RM} ${OBJ_DIR}

fclean: clean
	@echo "Cleaning all files..."
	@${RM} $(NAME)

re: fclean all
