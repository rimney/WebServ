/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 00:38:14 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/04/11 07:13:54 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/servers.hpp"

servers::servers() {}

servers::servers(servers const & s)
{
    *this = s;
}

servers::~servers() {}

servers &   servers::operator=(servers const & s)
{
    _set_fds = s._set_fds;
    _set_read_fds = s._set_read_fds;
    _max_fd = s._max_fd;
    _servers = s._servers;
    _fds_cnx = s._fds_cnx;
    return  *this;
}

int servers::setup(std::vector<server_parser> servers_config)
{
    size_t  i;

    for (i = 0; i < servers_config.size(); i++)
    {
        server  s(servers_config[i].getPortObject(), servers_config[i].getHostObject());
        for (size_t j = 0; j < s.get_port().size(); j++)
        {
            try
            {
                s.setup(servers_config[i], j);
                _servers.insert(std::make_pair(s.get_fd_socket(), s));
            }
            catch (std::string const & msg)
            {
                std::cout << msg << '\n';
                if (s.get_error_flag() == 1)
                {
                    s.close();
                    s.set_error_flag(0); 
                }
                _servers.erase(s.get_fd_socket());
            }
        }
    }

    FD_ZERO(&_set_fds);

    _max_fd = 0;
    for (std::map<int, server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        FD_SET((*it).first, &_set_fds);
        if (_max_fd < (*it).first)
            _max_fd = (*it).first;
    }
    if (_max_fd == 0)
    {
        std::cerr << "ERROR: couldn't run servers\n";
        return 1;
    }
    return 0;
}

void    servers::run()
{
    int             r;
    struct timeval  time;

    time.tv_sec = 10;
    time.tv_usec = 0;
    
    while(1)
    {
        FD_ZERO(&_set_read_fds);
        memcpy(&_set_read_fds, &_set_fds, sizeof(_set_fds));
        FD_ZERO(&_set_write_fds);
        for (std::vector<int>::iterator it = _fds_ready.begin(); it != _fds_ready.end(); it++)
            FD_SET(*it, &_set_write_fds);

        r = select(_max_fd + 1, &_set_read_fds, &_set_write_fds, NULL, &time);
        if (r == -1)
        {
            std::cerr << "ERROR: failed to select sockets.\n";
             
            for (std::map<int, server>::iterator it = _fds_cnx.begin(); it != _fds_cnx.end(); it++)
                ::close((*it).first);

            _fds_ready.clear();
            _fds_cnx.clear();
            FD_ZERO(&_set_read_fds);
            FD_ZERO(&_set_write_fds);
            FD_ZERO(&_set_fds);

            _max_fd = 0;
            for (std::map<int, server>::iterator it = _servers.begin(); it != _servers.end(); it++)
            {
                FD_SET((*it).first, &_set_fds);
                if (_max_fd < (*it).first)
                    _max_fd = (*it).first;
            }
            continue ;
        }
        else if (r == 0)
            continue ;

        // accept connections
        for (std::map<int, server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        {
            if (FD_ISSET((*it).first, &_set_read_fds))
            {
                try
                {
                    (*it).second.accept((*it).first);
                    FD_SET((*it).second.get_fd_connection(), &_set_fds);
                    _fds_cnx.insert(std::make_pair((*it).second.get_fd_connection(), (*it).second));
                    if (_max_fd < (*it).second.get_fd_connection())
                        _max_fd = (*it).second.get_fd_connection();
                    std::cout << "host: " << (*it).second.get_host() << ", port: " << (*it).second.get_fd_port((*it).first)
                        << " accept a new connections\n\n";
                }
                catch(const std::string& msg)
                {
                    std::cerr << msg << '\n';
                }
            }
        }

        // receive requests
        for (std::map<int, server>::iterator it = _fds_cnx.begin(); it != _fds_cnx.end(); it++)
        {
            if (FD_ISSET((*it).first, &_set_read_fds))
            {
                try
                {
                    (*it).second.receive((*it).first);
                    (*it).second.process((*it).first);
                    _fds_ready.push_back((*it).first);
                }
                catch(const std::string& msg)
                {
                    int fd = (*it).first ;
                    std::cerr << msg << "\n";
                    FD_CLR(fd, &_set_fds);
                    FD_CLR(fd, &_set_write_fds);
                    std::vector<int>::iterator i = std::find(_fds_ready.begin(), _fds_ready.end(), fd);
                    if (i != _fds_ready.end())
                        _fds_ready.erase(i);
                    _fds_cnx[fd].get_fd_port().erase(fd);
                    _fds_cnx.erase(fd);
                    close(fd);
                    break ;
                }
            }
        }

        // send response
        for (size_t i = 0; i < _fds_ready.size(); i++)
        {
            if (FD_ISSET(_fds_ready[i], &_set_write_fds))
            {
                try
                {
                        _fds_cnx[_fds_ready[i]].send(_fds_ready[i]);
                        if (_fds_cnx[_fds_ready[i]].getRespond(_fds_ready[i]).getBodyFlag() == false)
                        {   
                            if (_fds_cnx[_fds_ready[i]].get_request()[_fds_ready[i]].get_header()["Connection"] != "keep-alive")
                            {
                                close(_fds_ready[i]);
                                _fds_cnx[_fds_ready[i]].get_fd_port().erase(_fds_ready[i]);
                                _fds_cnx.erase(_fds_ready[i]);
                                FD_CLR(_fds_ready[i], &_set_fds);
                            }    
                            _fds_ready.erase(_fds_ready.begin() + i);
                        }
                }
                catch(const std::string& msg)
                {
                    std::cerr << msg << "\n";
                    close(_fds_ready[i]);
                    FD_CLR(_fds_ready[i], &_set_fds);
                    _fds_cnx[_fds_ready[i]].get_fd_port().erase(_fds_ready[i]);
                    _fds_cnx.erase(_fds_ready[i]);
                    _fds_ready.erase(_fds_ready.begin() + i);
                }
            }
        }
    }
}
