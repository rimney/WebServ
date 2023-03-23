

# include "../includes/cgi_handler.hpp"

cgi_handler::cgi_handler() {}

cgi_handler::cgi_handler(server_parser server_config, Request request)
    : _server_config(server_config), _request(request)
{
    init_env();
}

cgi_handler::cgi_handler(cgi_handler const & c)
{
    *this = c;
}

cgi_handler::~cgi_handler() {}

cgi_handler &   cgi_handler::operator=(cgi_handler const & c)
{
    _env = c._env;
}

std::string get_auth_type(std::string & auth_header)
{
    std::size_t pos = auth_header.find(' ');

    return auth_header.substr(0, pos);
}

void            cgi_handler::init_env()
{
    struct Start_line   start_line = _request.get_start_line();
    std::map<std::string,std::string> headers = _request.get_header();

    if (headers.find("Authorization") != headers.end())
        _env.push_back("AUTH_TYPE" + '=' + get_auth_type(headers["Authorization"]));
        _env.push_back("REMOTE_IDENT" + '=' + headers["Authorization"]);
        _env.push_back("REMOTE_USER" + '=' + headers["Authorization"]);
    
    if (headers.find("Content-Length") != headers.end())
        _env.push_back("CONTENT_LENGTH" + '=' + headers["Content-Length"]);
    
    if (headers.find("Content-Type") != headers.end())
        _env.push_back("CONTENT_TYPE" + '=' + headers["Content-Type"]);

    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    _env.push_back("PATH_INFO" + '=' + start_line.full_path);
    _env.push_back("PATH_TRANSLATED" + '=' + start_line.full_path);
    _env.push_back("QUERY_STRING" + '=' + start_line.query);
    // _env.push_back("REMOTE_ADDR" + '=' + _server_config.getHostObject());
    // _env.push_back("REMOTE_HOST" + ...);
    _env.push_back("REQUEST_METHOD" + '=' + start_line.method);
    _env.push_back("SCRIPT_NAME" + '=' + _server_config.getCgiPathObject());
    
    if (headers.find("Hostname") != headers.end())
        _env.push_back("SERVER_NAME" + '=' + headers["Hostname"]);
    else
        _env.push_back("SERVER_NAME" + '=' + _server_config.getHostObject());
    
    _env.push_back("SERVER_PORT" + '=' + _server_config.getPortObject());
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("SERVER_SOFTWARE=WebServ/0.0");
}

char**  cgi_handler::vector_to_ptr()
{
    char **env = new char*[_env.size() + 1];
    int i = 0;

    for (std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    {
        env[i] = new char[(*it).size() + 1];
        env[i] = strcpy(env[i], (*it).c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}

void            cgi_handler::exec()
{
    char ** env;

    env = get_ptr();

}