

#include "../../includes/servers.hpp"

servers::servers()
    : _servers_count(2)
{
    // config_parser   tmp_config(DEFAULT_CONFIG);
    // _config = tmp_config;
    setup();
}

// servers::servers(config_parser config)
//     : _config(config), _servers_count(_config.getServersCount())
// {
//     setup();
// }

servers::servers(servers const & s)
{
    *this = s;
}

servers::~servers()
{

}

servers &   servers::operator=(servers const & s)
{
    (void) s;
    return  *this;
}

void    servers::setup()
{
    int fd, i;

    for (i = 0; i < _servers_count; i++)
    {
        try
        {
            _servers.push_back(server(400 + i, INADDR_ANY));
        }
        catch(const std::string& msg)
        {
            std::cerr << msg << '\n';
            // handle error: close fds ... 
        }
    }

    FD_ZERO(&_set_fds);

    _max_fd = 0;
    for (i = 0; i < _servers_count; i++)
    {
        fd = _servers[i].get_fd_socket();
        FD_SET(fd, &_set_fds);
        if (_max_fd < _servers[i].get_fd_socket())
            _max_fd = _servers[i].get_fd_socket();
    }
}

void    servers::run()
{
    while(1)
    {
        memcpy(&_set_read_fds, &_set_fds, sizeof(_set_fds)); // use ft_memcpy()
        // _set_read_fds = _set_fds;
        if (select(_max_fd + 1, &_set_read_fds, NULL, NULL, NULL) == -1)
        {
            throw(std::string("ERROR: failed to select sockets"));
            // handle error: close fd's ... 
        }

        // accept connections
        for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        {
            if (FD_ISSET((*it).get_fd_socket(), &_set_read_fds))
            {
                try
                {
                    (*it).accept();
                }
                catch(const std::string& msg)
                {
                    std::cerr << msg << '\n';
                    // handle error: close fds ... 
                }
                
                FD_SET((*it).get_fd_connection(), &_set_fds);
                _fds_cnx.insert(std::make_pair((*it).get_fd_connection(), *it));
                if (_max_fd < (*it).get_fd_connection())
                    _max_fd = (*it).get_fd_connection();
                std::cout << "host: " << (*it).get_host() << ", port: " << (*it).get_port() << " accept a new connections\n";
                // break ;
            }
            // else
            //     FD_CLR((*it).get_fd_socket(), &_set_fds);
        }

        // receive requests
        for (std::map<int, server>::iterator it = _fds_cnx.begin(); it != _fds_cnx.end(); it++)
        {
            if (FD_ISSET((*it).first, &_set_read_fds))
            {
                try
                {
                    (*it).second.receive();
                }
                catch(const std::string& msg)
                {
                    std::cerr << msg << '\n';
                    // handle error: close fds ... 
                }
                std::cout << (*it).second.get_request() << '\n';
            }
        }

        // for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        //     close((*it).get_fd_connection());
    }
}

// void    servers::parse_conf(char* conf)
// {
//     config_parser   tmp_config(conf);
//     _config = tmp_config;
// }