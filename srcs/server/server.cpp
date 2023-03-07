

#include "../../includes/server.hpp"

server::server()
    : _port(DEFAULT_PORT), _host(INADDR_ANY), _error_flag(0)
{
    setup();
}

server::server(int port, unsigned int host)
    : _port(port), _host(host), _error_flag(0)
{
    setup();
}

server::server(server const & s)
    : _error_flag(0)
{
    *this = s;
}

server::~server() {}

int server::get_port() const
{
    return _port;
}

unsigned int    server::get_host() const
{
    return _host;
}

std::string   server::get_request() const
{
    return _request;
}

int server::get_fd_socket() const
{
    return _fd_socket;
}

int server::get_fd_connection() const
{
    return _fd_connection;
}

// TO BE EDITED TO ADD THE OTHER CLASS ARGS
server  & server::operator=(server const & s)
{
    _port = s._port;
    _host = s._host;
    _fd_socket = s._fd_socket;
    _fd_connection = s._fd_connection;
    _addr = s._addr;
    _request = s._request;
    return *this;
}

// to handle exception thrown by this method,
// I should set a flag or an enum to know what kind of error I am handling
void server::setup()
{
    int optval = 1;

    _fd_socket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (_fd_socket == -1)
    {
        _error_flag = 1;
        throw(std::string("ERROR: failed to create the socket."));
    }

    //Allow socket descriptor to be reuseable
    if (setsockopt(_fd_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        throw(std::string("ERROR: faild to set socket option (setsockopt()) for _fd_socket"));
    set_addr();
    if (bind(_fd_socket, (struct sockaddr*)&_addr, sizeof(_addr)) == -1)
        throw(std::string("ERROR: failed to bind the socket."));
    if (listen(_fd_socket, 100) == -1)
        throw(std::string("ERROR: failed to listen."));
    std::cout << "listening...\n";
}

void    server::set_addr()
{
    memset((char *)&_addr, 0, sizeof(_addr)); // use ft_memset() of libft
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = htonl(_host);
    _addr.sin_port = htons(_port);
}

void server::accept()
{
    _fd_connection = ::accept(_fd_socket, NULL, NULL);
    if (_fd_connection == -1)
        throw(std::string("ERROR: connection faild."));
    if (fcntl(_fd_connection, F_SETFL, O_NONBLOCK) == -1)
        throw(std::string("ERROR: fcntl() failed."));
}

void    server::close()
{
    ::close(_fd_socket);
    ::close(_fd_connection);
}

void    server::receive()
{
    int     rec;
    char    buffer[RECV_SIZE] = {0};
    
    rec = recv(_fd_connection, buffer, RECV_SIZE, 0);
    if (rec == -1)
        throw(std::string("ERROR: failed to receive data."));
    _request = std::string(buffer);
}
