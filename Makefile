NAME = webserv

SRC = webserv.cpp srcs/parsing/Parsing.cpp srcs/server/server.cpp
 
CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g3

${NAME} : ${SRC}
		@g++ ${CFLAGS} ${SRC} -o ${NAME}

all : ${NAME}

clean :
		@rm -rf ${NAME}

fclean : clean

re : fclean all