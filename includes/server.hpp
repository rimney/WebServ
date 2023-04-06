#ifndef SERVER_HPP
# define SERVER_HPP

# define DEFAULT_PROTOCOL 0
# define RECV_SIZE 1024
# define CHUNK_SIZE 1024 * 2

# include <netinet/in.h>
# include <string>
# include <unistd.h>
# include <iostream>
# include <sys/socket.h>
# include <vector>
# include <fcntl.h>
# include "parsing.hpp"
# include <cstring>
# include <sys/stat.h>
# include <stdio.h>
# include "request.hpp"
# include "respond.hpp"
# include "cgi_handler.hpp"
# include <sys/time.h>
#include <string.h>

class server
{
    private:
        std::vector<int>            _port;
        unsigned int                _host;
        int                         _fd_socket;
        int                         _fd_connection;
        struct sockaddr_in          _addr;
        server_parser               _server_config;
        int                         _error_flag;
        std::map<int, int>          _fd_port_map;
        std::map<int, std::string>  _request_map;
        std::map<int, respond>      _respond;
        std::map<int, Request>      _request;

    public:
        server();
        server(std::vector<int> port, unsigned int host);
        server(server const & s);
        ~server();

        std::vector<int>    get_port() const;
        int                 get_port(int i) const;
        unsigned int        get_host() const;
        std::map<int, respond>  get_respond() const;
        std::map<int, Request>  get_request() const;
        int                 get_fd_socket() const;
        int                 get_fd_connection() const;
        int                 get_error_flag() const;
        int                 get_fd_port(int fd);
        std::map<int, int>  get_fd_port();
        server  &           operator=(server const & s);
        void                accept(int fd);
        void                close();
        void                receive(int fd);
        void                send(int fd);
        void                set_server_config(server_parser & server_config);
        void                set_error_flag(int error_flag);
        void                setup(server_parser & server_config, size_t i);
        void                delete_method(std::string & path, int fd);
        server_parser       getServerData(void) {return this->_server_config;}
        void                process(int fd);
        void                Get(int location_index , std::string path, int fd);
        respond             getRespond(int fd);
        void                post_method(server_parser &serv, int fd);
        void                multi_part(server_parser &serv,int fd);
        server_parser       get_server_config();

    private:
        void            set_addr(int i);
};

#endif