/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 00:38:14 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/22 03:40:36 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/servers.hpp"

servers::servers(){}

servers::servers(config_parser config)
    : _servers_count(config.getServerCountObject()) {}

servers::servers(servers const & s)
{
    *this = s;
}

servers::~servers() {}

servers &   servers::operator=(servers const & s)
{
    _set_fds = s._set_fds;
    _set_read_fds = s._set_read_fds;
    _servers_count = s._servers_count;
    _max_fd = s._max_fd;
    _servers = s._servers;
    _fds_cnx = s._fds_cnx;
    return  *this;
}

int servers::setup(std::vector<server_parser> servers_config)
{
    int fd, i;

    for (i = 0; (size_t)i < servers_config.size(); i++)
    {
        try
        {
            _servers.push_back(server(servers_config[i].getPortObject(),
                servers_config[i].getHostObject(), servers_config[i]));
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
    for (i = 0; (size_t)i < servers_config.size(); i++)
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
    int r;
    int fd;
    struct timeval time;

    time.tv_sec = 5;
    time.tv_usec = 0;

    while(1)
    {
        // memcpy(&_set_read_fds, &_set_fds, sizeof(_set_fds)); // use ft_memcpy()
        _set_read_fds = _set_fds;
        FD_ZERO(&_set_write_fds);
        for (std::map<int, server>::iterator it = _fds_ready.begin(); it != _fds_ready.end(); it++)
            FD_SET((*it).first, &_set_write_fds);
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
            std::cout << "<< here >>\n";
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
                    _fds_ready.insert(std::make_pair((*it).first, (*it).second));
                    // break;
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
        for (std::map<int, server>::iterator it = _fds_ready.begin(); it != _fds_ready.end(); it++)
        {
            if (FD_ISSET((*it).first, &_set_write_fds))
            {
                try
                {
                    (*it).second.send((*it).first);
                    _fds_cnx.erase((*it).first);
                    _fds_ready.erase((*it).first);
                    break;
                }
                catch(const std::string& msg)
                {
                    std::cerr << msg << "\n";
                    FD_CLR((*it).first, &_set_write_fds);
                    FD_CLR((*it).first, &_set_read_fds);
                    break ;
                }
            }
        }
        
        // for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        //     close((*it).get_fd_connection());
    }
}
