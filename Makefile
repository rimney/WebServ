NAME = webserv

SERVER = srcs/server

SRC = webserv.cpp ${SERVER}/server.cpp ${SERVER}/servers.cpp 

CFLAGS = -Wall -Wextra -Werror -std=c++98

${NAME} : ${SRC}
		@g++ ${CFLAGS} ${SRC} -o ${NAME}

all : ${NAME}

clean :
		@rm -rf ${NAME}

fclean : clean

re : fclean all