


#ifndef CGI_HANDLER
# define CGI_HANDLER

# include <vector>
# include <unistd.h>
# include "request.hpp"
# include "parsing.hpp"

# define CGI_BUFFER 1024

class cgi_handler
{
    private:
        std::vector<std::string>    _env;
        server_parser               _server_config;
        Request                     _request;
        server_location             _location;
        

    public:
        cgi_handler(server_parser server_config, Request request);
        cgi_handler(cgi_handler const & c);
        ~cgi_handler();

        cgi_handler &   operator=(cgi_handler const & c);
        void            init_env();
        std::string     exec(std::string const & script);
    
    private:
        cgi_handler();
        char**          vector_to_ptr();

};



#endif