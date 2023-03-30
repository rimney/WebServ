/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 00:38:14 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/29 05:29:20 by eel-ghan         ###   ########.fr       */
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
    int     fd;
    size_t  i;

    for (i = 0; i < servers_config.size(); i++)
    {
        try
        {
            _servers.push_back(server(servers_config[i].getPortObject(),
                servers_config[i].getHostObject()));
            _servers[i].setup(servers_config[i]);
        }
        catch(const std::string& msg)
        {
            std::cerr << msg << '\n';
            if (_servers[i].get_error_flag())
                for (int j = 0; j < i + 1; j++)
                    _servers[j].close();
            else
                for (int j = 0; j < i; j++)
                    _servers[j].close();
            _servers.clear();
            return 1;
        }
    }

    FD_ZERO(&_set_fds);

    _max_fd = 0;
    for (i = 0; i < servers_config.size(); i++)
    {
        fd = _servers[i].get_fd_socket();
        FD_SET(fd, &_set_fds);
        if (_max_fd < _servers[i].get_fd_socket())
            _max_fd = _servers[i].get_fd_socket();
    }
    return 0;
}

void    servers::run()
{
    int             r;
    int             fd;
    struct timeval  time;

    time.tv_sec = 10;
    time.tv_usec = 0;

    while(1)
    {
        _set_read_fds = _set_fds;
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
            FD_ZERO(&_set_fds);
            FD_ZERO(&_set_read_fds);
            FD_ZERO(&_set_write_fds);
            _max_fd = 0;
            for (int i = 0; (size_t)i < _servers.size(); i++)
            {
                fd = _servers[i].get_fd_socket();
                FD_SET(fd, &_set_fds);
                if (_max_fd < _servers[i].get_fd_socket())
                    _max_fd = _servers[i].get_fd_socket();
            }
            continue ;
        }
        else if (r == 0)
            continue ;

        // accept connections
        for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        {
            if (FD_ISSET((*it).get_fd_socket(), &_set_read_fds))
            {
                try
                {
                    (*it).accept();
                    FD_SET((*it).get_fd_connection(), &_set_fds);
                    _fds_cnx.insert(std::make_pair((*it).get_fd_connection(), *it));
                    if (_max_fd < (*it).get_fd_connection())
                        _max_fd = (*it).get_fd_connection();
                    std::cout << "host: " << (*it).get_host() << ", port: " << (*it).get_port() << " accept a new connections\n\n";
                }
                catch(const std::string& msg)
                {
                    std::cerr << msg << '\n';
                    break ;
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
                    std::cerr << msg << "\n";
                    FD_CLR((*it).first, &_set_read_fds);
                    FD_CLR((*it).first, &_set_fds);
                    _fds_cnx.erase((*it).first);
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
                        _fds_ready.erase(_fds_ready.begin() + i);
                }
                catch(const std::string& msg)
                {
                    std::cerr << msg << "\n";
                    // FD_CLR((*it).first, &_set_write_fds);
                    // FD_CLR((*it).first, &_set_read_fds);
                    break ;
                }
            }
        }
        
        // for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        //     close((*it).get_fd_connection());
    }
}
