/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 00:38:14 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/04/04 04:51:56 by eel-ghan         ###   ########.fr       */
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
    // int             fd;
    struct timeval  time;

    time.tv_sec = 10;
    time.tv_usec = 0;
    
    while(1)
    {
        memcpy(&_set_read_fds, &_set_fds, sizeof(_set_fds));
        memcpy(&_set_error_fds, &_set_fds, sizeof(_set_fds));
        FD_ZERO(&_set_write_fds);
        for (std::map<int, server>::iterator it = _fds_ready.begin(); it != _fds_ready.end(); it++)
            FD_SET((*it).first, &_set_write_fds);
            
        r = select(_max_fd + 1, &_set_read_fds, &_set_write_fds, &_set_error_fds, &time);

        if (r == -1)
        {
            std::cout << "ERROR: failed to select sockets.\n";
            for (std::map<int, server>::iterator it = _fds_cnx.begin(); it != _fds_cnx.end(); it++)
                ::close((*it).first);
            _fds_ready.clear();
            _fds_cnx.clear();
            FD_ZERO(&_set_read_fds);
            FD_ZERO(&_set_write_fds);
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
                    std::cout << "accept <<<\n";
                    FD_SET((*it).second.get_fd_connection(), &_set_fds);
                    _fds_cnx.insert(std::make_pair((*it).second.get_fd_connection(), (*it).second));
                    if (_max_fd < (*it).second.get_fd_connection())
                        _max_fd = (*it).second.get_fd_connection();
                    std::cout << "host: " << (*it).second.get_host() << ", port: " << (*it).second.get_fd_port((*it).first)
                        << " accept a new connections\n\n";
                }
                catch(const std::string& msg)
                {
                    std::cout << msg << '\n';
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
                    if((*it).second.get_server_config().getServerErrorPageObject().empty())
                    {
                        std::cout << "SERVER NOT FOUND\n";
                        exit(0);
                    }
                    (*it).second.receive((*it).first);
                    std::cout << "receive <<<\n";
                    (*it).second.process((*it).first);
                    // _fds_ready.push_back((*it).first);
                    _fds_ready.insert(std::make_pair((*it).first, (*it).second));
                }
                catch(const std::string& msg)
                {
                    int fd = (*it).first ;
                    std::cout << msg << ", fd: " << (*it).first << "\n";
                    std::cout << "errno: " << errno << "\n";
                    perror("recv");
                    FD_CLR(fd, &_set_write_fds);
                    FD_CLR(fd, &_set_read_fds);
                    FD_CLR(fd, &_set_fds);
                    if (_fds_ready.find(fd) != _fds_ready.end())
                        _fds_ready.erase(fd);
                    _fds_cnx.erase(fd);
                    close(fd);
                    break ;
                }
            }
        }


        for (std::map<int, server>::iterator it = _fds_ready.begin(); it != _fds_ready.end(); it++)
        {
            if (FD_ISSET((*it).first, &_set_write_fds))
            {
                try
                {
                    // if (!FD_ISSET((*it).first, &_set_error_fds))
                    // {
                        (*it).second.send((*it).first);
                        // std::cout << "send <<<\n";
                        if ((*it).second.getRespond((*it).first).getBodyFlag() == false)
                        {
                            _fds_ready.erase(it);
                            // _fds_cnx[i].get_fd_port().erase((*it).first);
                            std::cout << "send <<<\n";
                        }
                    // }
                    // else
                    //     FD_CLR((*it).first, &_set_write_fds);
                    break;
                }
                catch(const std::string& msg)
                {
                    int fd = (*it).first;
                    std::cout << msg << ", fd: " << (*it).first << "\n";
                    std::cout << "errno: " << errno << ", perror: ";
                    perror("send");
                    FD_CLR((*it).first, &_set_write_fds);
                    FD_CLR((*it).first, &_set_read_fds);
                    (*it).second.get_fd_port().erase((*it).first);
                    // if (_fds_ready.find((*it).first) != _fds_ready.end())
                        _fds_ready.erase(fd);
                    _fds_cnx.erase(fd);
                    break ;
                }
            }
        }
        
        
        // send response
        // for (size_t i = 0; i < _fds_ready.size(); i++)
        // {
        //     if (FD_ISSET(_fds_ready[i], &_set_write_fds))
        //     {
        //         try
        //         {
        //             // if (!FD_ISSET(_fds_ready[i], &_set_error_fds))
        //             // {
        //                 _fds_cnx[_fds_ready[i]].send(_fds_ready[i]);
        //                 // std::cout << "send <<<\n";
        //                 if (_fds_cnx[_fds_ready[i]].getRespond(_fds_ready[i]).getBodyFlag() == false)
        //                 {
        //                     _fds_ready.erase(_fds_ready.begin() + i);
        //                     std::cout << "send <<<\n";
        //                     // _fds_cnx[i].get_fd_port().erase(_fds_ready[i]);
        //                 }
        //             // }
        //             // else
        //             //     FD_CLR(_fds_ready[i], &_set_write_fds);
        //             // break;
        //         }
        //         catch(const std::string& msg)
        //         {
        //             std::cout << msg << ", fd: " << _fds_ready[i] << "\n";
        //             std::cout << "errno: " << errno << ", perror: ";
        //             perror("send");
        //             FD_CLR(_fds_ready[i], &_set_write_fds);
        //             FD_CLR(_fds_ready[i], &_set_read_fds);
        //             _fds_cnx[i].get_fd_port().erase(_fds_ready[i]);
        //             _fds_ready.erase(_fds_ready.begin() + i);
        //             _fds_cnx.erase(i);
        //             break ;
        //         }
        //     }
        // }
        
        // for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        //     close((*it).get_fd_connection());
    }
}
