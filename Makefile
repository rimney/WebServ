NAME = webserv
 
SERVER = srcs/server

SRC = 	webserv.cpp ${SERVER}/server.cpp ${SERVER}/servers.cpp srcs/parsing/Parsing.cpp ${SERVER}/post_method.cpp\
		srcs/request/request.cpp  srcs/respond/respond.cpp  srcs/CGI/cgi_handler.cpp

CFLAGS = -Wall -Wextra -Werror -std=c++98 -stdlib=libc++

${NAME} : ${SRC}
		@clang++ ${CFLAGS} ${SRC} -o ${NAME}

all : ${NAME}

clean :
		@rm -rf ${NAME}

fclean : clean

re : fclean all