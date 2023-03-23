


#ifndef CGI_HANDLER
# define CGI_HANDLER

# include <vector>
# include "request.hpp"
# include "parsing.hpp"

class cgi_handler
{
    private:
        std::vector<std::string>    _env;
        server_parser               _server_config;
        Request                     _request;
        

    public:
        cgi_handler(server_parser server_config, Request request);
        cgi_handler(cgi_handler const & c);
        ~cgi_handler();

        cgi_handler &   operator=(cgi_handler const & c);
        void            init_env();
        void            exec();
    
    private:
        cgi_handler();
        char**  cgi_handler::vector_to_ptr();

};



#endif