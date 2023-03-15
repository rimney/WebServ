

#ifndef SERVER_HPP
# define SERVER_HPP

# define DEFAULT_PORT 8080
# define DEFAULT_PROTOCOL 0
# define RECV_SIZE 1024


# include <netinet/in.h>
# include <string>
# include <unistd.h>
# include <iostream>
# include <sys/socket.h>
# include <vector>
# include <fcntl.h>
# include "parsing.hpp"
# include "request.hpp"
# include <cstring>
# include <sys/stat.h>
# include <stdio.h>

// TO BE DELETED //
#include <string.h>
//______________//

class server
{
    private:
        int                 _port;
        unsigned int        _host;
        int                 _fd_socket;
        int                 _fd_connection;
        struct sockaddr_in  _addr;
        server_parser       _server_config;
        int                 _error_flag;
        std::string         _request;
        std::string         _response;
        Request             request;

    public:
        server();
        server(int port, unsigned int host);
        server(server const & s);
        ~server();

        int             get_port() const;
        unsigned int    get_host() const;
        std::string     get_request() const;
        int             get_fd_socket() const;
        int             get_fd_connection() const;
        int             get_error_flag() const;
        server  &       operator=(server const & s);
        void            accept();
        void            close();
        void            receive();
        void            set_server_config(server_parser & server_config);
        void            setup(server_parser & server_config);
        void            delete_method(std::string & path);
        void            process();
    
    private:
        void            set_addr();
};

#endif