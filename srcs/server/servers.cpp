

#include "servers.hpp"

servers::servers()
{
    config_parser   tmp_config(DEFAULT_CONFIG);
    _config = tmp_config;
    setup();
}

servers::servers(config_parser config)
    : _config(config)
{
    setup();
}

servers::servers(servers const & s)
{
    *this = s;
}

servers::~servers()
{

}

servers &   servers::operator=(servers const & s)
{

}

void    servers::setup()
{
    for (int i = 0; i < _config.getServersCount(); i++)
    {
        
    }
}

// void    servers::parse_conf(char* conf)
// {
//     config_parser   tmp_config(conf);
//     _config = tmp_config;
// }
