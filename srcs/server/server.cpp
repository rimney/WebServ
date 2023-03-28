/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 00:38:09 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/24 17:01:50 by rimney            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/server.hpp"

server::server()
    : _port(DEFAULT_PORT), _host(INADDR_ANY), _error_flag(1){}

server::server(int port, unsigned int host)
    : _port(port), _host(host) ,_error_flag(1) {}

server::server(server const & s)
    : _error_flag(1)
{
    *this = s;
}

respond server::getRespond(int fd)
{
    return _respond[fd];
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

// std::string   server::get_request() const
// {
//     return _request;
// }

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

server  & server::operator=(server const & s)
{
    _port = s._port;
    _host = s._host;
    _fd_socket = s._fd_socket;
    _fd_connection = s._fd_connection;
    _addr = s._addr;
    _server_config = s._server_config;
    _request_map = s._request_map;
    _request = s._request;
    _respond = s._respond;
    // this->respond.setRespondServer(_server_config);
    return *this;
}

void server::setup(server_parser & server_config)
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
    {
        ::close(_fd_connection);
        throw(std::string("ERROR: fcntl() failed."));
    }
}

void    server::close()
{
    ::close(_fd_socket);
}

void    server::receive(int fd)
{
    int     r;
    char    buffer[RECV_SIZE] = {0};
    
    r = recv(fd, buffer, RECV_SIZE, 0);
    if (r == -1)
    {
        ::close(fd);
        throw(std::string("ERROR: failed to receive data, closing connection."));
    }
    else if (r == 0)
    {
        ::close(fd);
        throw(std::string("NOTE: connection closed by client."));
    }
    if (_request_map.find(fd) != _request_map.end())
    {
        _request_map[fd].clear();
        _request_map[fd] = std::string(buffer, r);
    }
    else
    {
        _request_map.insert(std::make_pair(fd, std::string(buffer, r)));
        _request.insert(std::make_pair(fd, Request()));
        _respond.insert(std::make_pair(fd, respond(_server_config)));
    }
}

void    server::send(int fd)
{
    // ssize_t sent;

    // std::cout << _respond[fd].getBody();
    // std::cout << _respond[fd].getBodyFlag() << " <<\n";
    if(_respond[fd].getBody().empty())
        _respond[fd].recoverBody(atoi(_respond[fd].getstatusCode().c_str()));
    
    if(_respond[fd].getBodyFlag() == true)
        _respond[fd].setFinalString(_respond[fd].chunkedFileToString(_respond[fd].getPathSave()));
    if(_respond[fd].getfinalString().size() > 0)
    {
        if ((::send(fd, _respond[fd].getfinalString().c_str(), _respond[fd].getfinalString().size(), 0)) == -1)
        {
            throw(std::string("ERROR: send() faild to send response"));
        }
    }
        
        _respond[fd].cleanAll();
}

void    server::set_server_config(server_parser  & server_config)
{
    _server_config = server_config;
}

int is_path_exist(std::string & path)
{
    if (access(path.c_str(), F_OK) == 0)
        return 1;
    return 0;
}

int is_file_or_dir(std::string & path)
{
    struct stat file_info;

    if (stat(path.c_str(), &file_info) == -1)
        return -1;
    if (file_info.st_mode & S_IFDIR)
        return 2;
    if (file_info.st_mode & S_IFREG)
        return 1;
    return -1;
}

void    server::delete_method(std::string  & path)
{
    
    if (is_path_exist(path))
    {
        int r = is_file_or_dir(path);
        if (r == 1) // handle file cases
        {
            // if location has cgi
            // call cgi to handle this case
            // else if :
            if (remove(path.c_str()) == 0)
            {
                std::cout << "HTTP/1.1 204 No Content\r\n";
                std::cout << "Content-Type: text/plain\r\n";
                std::cout << "\r\n";
                return ;
            }
            std::cout << "HTTP/1.1 500 Internal Server Error\r\n";
            std::cout << "Content-Type: text/plain\r\n";
            std::cout << "\r\n";
            std::cout << "Internal Server Error\n";
            return ;
        }
        else if (r == 2) // handle dir cases
        {
            if (path.back() == '/')
            {
                // if location has cgi
                // call cgi to handle this case
                // else if :
                if (access(path.c_str(), W_OK) == 0)
                {
                    if (remove(path.c_str()) == 0)
                    {
                        std::cout << "HTTP/1.1 204 No Content\r\n";
                        std::cout << "Content-Type: text/plain\r\n";
                        std::cout << "\r\n";
                        return ;
                    }
                    std::cout << "500 Internal Server Error\r\n";
                    std::cout << "Content-Type: text/plain\r\n";
                    std::cout << "\r\n";
                    std::cout << "Internal Server Error\n";
                    return ;
                }
                std::cout << "HTTP/1.1 403 Forbidden\r\n";
                std::cout << "Content-Type: text/plain\r\n";
                std::cout << "\r\n";
                std::cout << "Forbidden\n";
                return ;
            }
            else
            {
                std::cout << "HTTP/1.1 409 Conflict\r\n";
                std::cout << "Content-Type: text/plain\r\n";
                std::cout << "\r\n";
                std::cout << "Conflict\n";
                return ;
            }
        }
        else // error
        {
            std::cout << "500 Internal Server Error\r\n";
            std::cout << "Content-Type: text/plain\r\n";
            std::cout << "\r\n";
            std::cout << "Internal Server Error\n";
            return ;
        }
    }
    std::cout << "HTTP/1.1 404 Not Found\r\n";
    std::cout << "Content-Type: text/plain\r\n";
    std::cout << "\r\n";
    std::cout << "Not Found\n";
}

void server::Get(int location_index , std::string path, int fd) 
{
    server_location location = _server_config.getOneLocationObject(location_index);
    std::string isFOrD = isFileOrDirectory(path);
    if(location.getHasRedirection())
    {
        _respond[fd].setBody(_respond[fd].fileToSring(location.getLocationRedirectionObject()));
        _respond[fd].setContentLenght(std::to_string(_respond[fd].getBody().size()));
        _respond[fd].mergeRespondStrings();
        return ;
    }
    else if(_respond[fd].getstatusCode() == "200" && _respond[fd].getBodyFlag() == false)
    {
        if(isFOrD == "file")
        {
            if(location.getHasCgi())
            {
                std::cout << "location has CGI !!\n";  // BARAE << 
                return ;
            }
            else
            {
                
                if(_respond[fd].fileToSring(path).size() > 50000 || _respond[fd].getBodyFlag() == true)
                {
                    if(_respond[fd].getBodyFlag() == true)
                    {
                        return ;
                    }
                    _respond[fd].setBodyFlag(true);
                    return;
                }
                else
                {
                    std::cout << path << " < path\n";
                    _respond[fd].setBody(_respond[fd].fileToSring(path));
                    _respond[fd].setContentLenght(std::to_string(_respond[fd].fileToSring(path).size()));
                    _respond[fd].mergeRespondStrings();
                }
                
                return ;
            }
        }
        else if(isFOrD == "directory")
        {
            std::cout << "HERE\n";
            if(isFileOrDirectory(location.getLocationRootObject() + "/" + location.getLocationIndexObject()) == "file")
            {
                Get(location_index, location.getLocationRootObject() + "/" + location.getLocationIndexObject(), fd);
            }
            else if (location.getLocationIndexObject().size() > 0 &&
                isFileOrDirectory(location.getLocationRootObject() + "/" + location.getLocationIndexObject()) == "error")
            {
                _respond[fd].setRespond(path, _respond[fd].gethttpVersion(), "404");
                return ;
            }
            else if(location.getLocationIsAutoIndexObject()) // HERE
            {
                _respond[fd].setBody(_respond[fd].getAutoIndexPage(path));
                _respond[fd].setContentLenght(std::to_string(_respond[fd].getBody().size()));
                _respond[fd].mergeRespondStrings();
                return ;
            }
            else
            {
                _respond[fd].setRespond(path, _respond[fd].gethttpVersion(), "403");
                return ;
            }
        }
    }
}

void    server::process(int fd)
{
    if(!_request[fd].get_wait_body())
        _request[fd].parser(_request_map[fd]);
    else
    {
        _request[fd].body_handling(_request_map[fd]);
    }
    if(!_request[fd].get_wait_body())
    {
        _request[fd].errors(_server_config);
        
        // std::cout << "\nThe first line is : \n";
        std::cout << "//////////////// REQUEST ///////////////////\n";
        std::cout <<  _request[fd].get_start_line().vertion << std::endl;
        std::cout <<  _request[fd].get_start_line().method << std::endl;
        std::cout <<  _request[fd].get_start_line().path << std::endl;
        std::cout <<  _request[fd].get_start_line().vertion << std::endl;
        std::cout <<  _request[fd].get_start_line().full_path << std::endl;
        std::cout <<  _request[fd].get_start_line().query << std::endl;
        std::cout << _request[fd].get_error() << "\n";
        std::cout << "//////////////// REQUEST ///////////////////\n\n";
        
        _respond[fd].setRespond(_request[fd].get_start_line().full_path, _request[fd].get_start_line().vertion, _request[fd].get_error());
        
        if(_request[fd].get_error().empty() || _respond[fd].getstatusCode() == "301")
        {
            if(_request[fd].get_start_line().method == "GET")
            {
                Get(_request[fd].get_start_line().location_index, _request[fd].get_start_line().full_path, fd);
            }
            if(_request[fd].get_start_line().method == "POST")
            {
                //
            }
            if(_request[fd].get_start_line().method == "DELETE")
            {
                delete_method(_request[fd].get_start_line().full_path);
            }
        }
        //respond  
        // _request[fd].clear();
        _request_map.erase(fd);
    }
}