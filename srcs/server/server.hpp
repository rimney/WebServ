

#ifndef SERVER_HPP
# define SERVER_HPP

# define DEFAULT_PORT 80
# define DEFAULT_PROTOCOL 0
# define RECV_SIZE 65536


# include <netinet/in.h>
# include <string>
# include <unistd.h>
# include <iostream>
# include <sys/socket.h>

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
        std::string         _request;

    public:
        server();
        server(int port, unsigned int host);
        server(server const & s);

        ~server();

        int             get_port() const;
        unsigned int    get_host() const;
        std::string     get_request() const;
        server  &       operator=(server const & s);
        void            accept();
        void            close();
        void            receive();
        void            run();
    
    private:
        void            setup();
        void            set_addr();
};

#endif