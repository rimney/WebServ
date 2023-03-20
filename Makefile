NAME = webserv
 
SERVER = srcs/server

SRC = webserv.cpp ${SERVER}/server.cpp ${SERVER}/servers.cpp srcs/parsing/Parsing.cpp  srcs/request/request.cpp srcs/respond/respond.cpp  ${SERVER}/post_method.cpp

CFLAGS = -Wall -Wextra -Werror -std=c++98 -stdlib=libc++ -fsanitize=address -g3

${NAME} : ${SRC}
		@clang++ ${CFLAGS} ${SRC} -o ${NAME}

all : ${NAME}

clean :
		@rm -rf ${NAME}

fclean : clean

re : fclean all