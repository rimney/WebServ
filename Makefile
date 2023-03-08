NAME = webserv
 
SERVER = srcs/server

SRC = webserv.cpp ${SERVER}/server.cpp ${SERVER}/servers.cpp srcs/parsing/parsing.cpp

CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g3

${NAME} : ${SRC}
		@g++ ${CFLAGS} ${SRC} -o ${NAME}

all : ${NAME}

clean :
		@rm -rf ${NAME}

fclean : clean

re : fclean all