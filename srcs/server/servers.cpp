

#include "../../includes/servers.hpp"

servers::servers()
    // : _servers_count(2)
{
    // config_parser   tmp_config(DEFAULT_CONFIG);
    // _config = tmp_config;
    // setup();
}

servers::servers(config_parser config)
    : _servers_count(config.getServerCountObject()) {}

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

int    servers::setup(server_parser *servers_config)
{
    int fd, i;

    for (i = 0; i < _servers_count; i++)
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
    for (i = 0; i < _servers_count; i++)
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

    while(1)
    {
        memcpy(&_set_read_fds, &_set_fds, sizeof(_set_fds)); // use ft_memcpy()
        r = select(_max_fd + 1, &_set_read_fds, NULL, NULL, NULL);
        if (r == -1)
        {
            std::cerr << "ERROR: failed to select sockets.\n";
            for (std::map<int, server>::iterator it = _fds_cnx.begin(); it != _fds_cnx.end(); it++)
                ::close((*it).first);
            _fds_cnx.clear();
            FD_ZERO(&_set_fds);
            _max_fd = 0;
            for (int i = 0; i < _servers_count; i++)
            {
                fd = _servers[i].get_fd_socket();
                FD_SET(fd, &_set_fds);
                if (_max_fd < _servers[i].get_fd_socket())
                    _max_fd = _servers[i].get_fd_socket();
            }
            continue ;
        }
        else if (r == 0)
        {
            std::cerr << "TIMEOUT\n";
            continue ;
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
                }
                
                FD_SET((*it).get_fd_connection(), &_set_fds);
                _fds_cnx.insert(std::make_pair((*it).get_fd_connection(), *it));
                if (_max_fd < (*it).get_fd_connection())
                    _max_fd = (*it).get_fd_connection();
                std::cout << "host: " << (*it).get_host() << ", port: " << (*it).get_port() << " accept a new connections\n\n";
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
                    FD_CLR((*it).first, &_set_read_fds);
                    FD_CLR((*it).first, &_set_fds);
                    _fds_cnx.erase((*it).first);
                }
                std::cout << (*it).second.get_request() << "\n\n";
            }
        }

        // for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        //     close((*it).get_fd_connection());
    }
}
