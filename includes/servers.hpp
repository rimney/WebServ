

#ifndef SERVERS_HPP
# define SERVERS_HPP

# include <sys/select.h>
# include <map>
// # include "webserv.hpp"
# include "server.hpp"
# include "parsing.hpp"

class servers
{
    private:
        fd_set                  _set_fds;
        fd_set                  _set_read_fds;
        fd_set                  _set_write_fds;
        int                     _max_fd;
        std::map<int, server>   _servers;
        std::map<int, server>   _fds_cnx;
        std::vector<int>        _fds_ready;

    public:
        servers();
        servers(servers const & s);
        ~servers();

        servers &   operator=(servers const & s);

        int    setup(std::vector<server_parser> servers_config);
        void    run();
};

#endif