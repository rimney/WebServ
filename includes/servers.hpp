

#ifndef SERVERS_HPP
# define SERVERS_HPP

# include <sys/select.h>
# include <map>
# include "webserv.hpp"
# include "server.hpp"
# include "parsing.hpp"

class servers
{
    private:
        fd_set                  _set_fds;
        fd_set                  _set_read_fds;
        // fd_set                  _set_write_fds;
        // config_parser           _config;
        int                     _servers_count;
        int                     _max_fd;
        std::vector<server>     _servers;
        std::map<int, server>   _fds_cnx;

    public:
        servers();
        // servers(config_parser config);
        servers(servers const & s);
        ~servers();

        servers &   operator=(servers const & s);

        void    run();

    private:
        void    setup();
};

#endif