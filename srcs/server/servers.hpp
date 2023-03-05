

#ifndef SERVERS_HPP
# define SERVERS_HPP



# include <sys/select.h>
# include "../../includes/webserv.hpp"
# include "../parsing/parsing.hpp"

class servers
{
    private:
        fd_set          _set;
        config_parser   _config;

    public:
        servers();
        servers(config_parser config);
        servers(servers const & s);
        ~servers();

        servers &   operator=(servers const & s);

    private:
        void    setup();
        // void    parse_conf(char *conf);
};

#endif