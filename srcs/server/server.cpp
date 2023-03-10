

#include "../../includes/server.hpp"


server::server()
    : _port(DEFAULT_PORT), _host(INADDR_ANY), _error_flag(1){}

server::server(int port, unsigned int host)
    : _port(port), _host(host), _error_flag(1) {}

server::server(server const & s)
    : _error_flag(1)
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

int server::get_error_flag() const
{
    return _error_flag;
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
void server::setup(server_parser server_config)
{
    int optval = 1;

    _fd_socket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (_fd_socket == -1)
    {
        _error_flag = 0;
        throw(std::string("ERROR: failed to create the socket."));
    }
    //Allow socket descriptor to be reuseable
    if (setsockopt(_fd_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        throw(std::string("ERROR: faild to set socket option (setsockopt()) for _fd_socket."));
    set_addr();
    if (bind(_fd_socket, (struct sockaddr*)&_addr, sizeof(_addr)) == -1)
        throw(std::string("ERROR: failed to bind the socket."));
    if (listen(_fd_socket, 100) == -1)
        throw(std::string("ERROR: failed to listen."));
    set_server_config(server_config);
    std::cout << "host: " << _host << " is listening on port " << _port << "...\n\n";
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
}

void    server::receive()
{
    int     rec;
    char    buffer[RECV_SIZE] = {0};
    
    rec = recv(_fd_connection, buffer, RECV_SIZE, 0);
    if (rec == -1)
        throw(std::string("ERROR: failed to receive data."));
    _request = std::string(buffer,rec);
}

void    server::set_server_config(server_parser server_config)
{
    _server_config = server_config;
}
void    server::process()
{
    if(!request.get_wait_body())
        request.parser(_request);
    else
    {
        request.body_handling(_request);
    }
    if(!request.get_wait_body())
    {
        std::cout << "\nThe first line is : \n";
        std::cout <<  request.get_start_line().method << std::endl;
        std::cout <<  request.get_start_line().path << std::endl;
        std::cout <<  request.get_start_line().vertion << std::endl;

        std::cout << "\n\n";
        std::map<std::string, std::string>::iterator itr;
        std::cout << "\nThe heder is : \n";
        std::cout << "\tKEY\tELEMENT\n";
        for (itr = request.get_header().begin(); itr != request.get_header().end(); ++itr) {
            std::cout << '\t' << "*"<< itr->first << "*" << '\t' <<  "*" << itr->second << "*"<< '\n';
        }
        std::cout << std::endl;
        if(!request.get_body().empty())
        {
            std::cout << "\nThe body is : \n"; 
            std::cout << "*" << request.get_body() << "*"<< std::endl;

        }
    }
}