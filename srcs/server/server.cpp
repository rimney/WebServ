

#include "server.hpp"
#include "../../includes/request.hpp"

server::server()
    : _port(DEFAULT_PORT), _host(INADDR_ANY)
{
    setup();
}

server::server(int port, unsigned int host)
    : _port(port), _host(host)
{
    setup();
}

server::server(server const & s)
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

// TO BE EDITED TO ADD THE OTHER CLASS ARGS
server  & server::operator=(server const & s)
{
    _port = s.get_port();
    _host = s.get_host();
    return *this;
}

void server::setup()
{
    _fd_socket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (_fd_socket == -1)
        throw(std::string("ERROR: failed to create the socket."));
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
}

void    server::close()
{
    ::close(_fd_socket);
    ::close(_fd_connection);
}

void    server::receive()
{
    Request request;
    int     rec;
    char    buffer[RECV_SIZE] = {0};
    
    rec = recv(_fd_connection, buffer, RECV_SIZE, 0);
    if (rec == -1)
    {
        close();
        throw(std::string("ERROR: failed to receive data"));
    }
    _request = std::string(buffer);
    request = _request;
    std::cout << "\nThe first line is : \n";
    std::cout <<  request.get_start_line().method << std::endl;
    std::cout <<  request.get_start_line().path << std::endl;
    std::cout <<  request.get_start_line().vertion << std::endl;
    std::cout << "\n\n";
    std::map<std::string, std::string>::iterator itr;
    std::cout << "\nThe heder is : \n";
    std::cout << "\tKEY\tELEMENT\n";
    for (itr = request.get_header().begin(); itr != request.get_header().end(); ++itr) {
        std::cout << '\t' << itr->first << '\t' << itr->second << '\n';
    }
    std::cout << std::endl;
    std::vector<std::string>::iterator itrv;
    std::cout << "\nThe body is : \n";
    for (itrv = request.get_body().begin(); itrv != request.get_body().end(); ++itrv) {
        std::cout << *itrv << '\n';
    }
}

void    server::run()
{
    while (1)
    {
        try
        {
            accept();
            receive();
            // std::cout << _request << '\n';
        }
        catch(std::string const & msg)
        {
            std::cout << msg << '\n';
        }
        if (_fd_connection >= 0)
            ::close(_fd_connection);
    }
    close();
}