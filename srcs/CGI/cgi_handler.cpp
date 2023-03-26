

# include "../../includes/cgi_handler.hpp"

cgi_handler::cgi_handler() {}

cgi_handler::cgi_handler(server_parser server_config, Request request)
    : _server_config(server_config), _request(request),
    _location(_server_config.getServerLocationsObject()[start_line.location_index])
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
    return *this;
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
        _env.push_back(std::string("AUTH_TYPE=") + get_auth_type(headers["Authorization"]));

    _env.push_back(std::string("REMOTE_IDENT=") + headers["Authorization"]);
    _env.push_back(std::string("REMOTE_USER=") + headers["Authorization"]);
    // _env.push_back(std::string("REQUEST_URI=") + )
    
    if (headers.find("Content-Length") != headers.end())
        _env.push_back(std::string("CONTENT_LENGTH=") + headers["Content-Length"]);
    
    if (headers.find("Content-Type") != headers.end())
        _env.push_back(std::string("CONTENT_TYPE=") + headers["Content-Type"]);

    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    _env.push_back(std::string("PATH_INFO=") + start_line.full_path);
    _env.push_back(std::string("PATH_TRANSLATED=") + start_line.full_path);
    _env.push_back(std::string("QUERY_STRING=") + start_line.query);
    // _env.push_back(std::string("REMOTE_ADDR=") + _server_config.getHostObject());
    // _env.push_back(std::string("REMOTE_HOST" + ...);
    _env.push_back(std::string("REQUEST_METHOD=") + start_line.method);
    _env.push_back(std::string("SCRIPT_NAME=") + _location.getCgiPathObject());
    
    if (headers.find("Hostname") != headers.end())
        _env.push_back(std::string("SERVER_NAME=") + headers["Hostname"]);
    else
        _env.push_back(std::string("SERVER_NAME=") + std::to_string(_server_config.getHostObject()));
    
    _env.push_back(std::string("SERVER_PORT=") + std::to_string(_server_config.getPortObject()));
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("SERVER_SOFTWARE=WebServ/0.0");

    for  (std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++)
        std::cout << *it << '\n';
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

std::string cgi_handler::exec()
{
    pid_t       cgi_pid;
    int         fds[2];
    char**      env;
    std::string body;
    // int         std_in, std_out;

    env = vector_to_ptr();

    // std_in = dup(STDIN_FILENO);
	// std_out = dup(STDOUT_FILENO);

    pipe(fds);

    cgi_pid = fork();

    if (cgi_pid == -1)
    {
        std::cerr << "ERROR: fork() failed\n";
        return "500 Intenal Server";
    }
    else if (cgi_pid == 0)
    {
        dup2(STDOUT_FILENO, fds[1]); //handle error when r = -1;
        dup2(STDIN_FILENO, fds[0]); //handle error when r = -1;
        std::cout << "<<< cgi-process >>> \n";
        // execve(_location.getCgiPathObject().c_str(),  NULL, env);
        execve("/bin/ls",  NULL, NULL);
        std::cerr << "ERROR: execve() failed\n";
        write(fds[1], "500 Intenal Server", 18);
    }
    else
    {
        char    buffer[CGI_BUFFER] = {0};
        int     r = 1;
    
        std::cout << "<<< parent-process-1 >>> \n";
        waitpid(-1, NULL, 0);
        std::cout << "<<< parent-process-2 >>> \n";
        while (r > 0)
        {
            memset(buffer, 0, CGI_BUFFER);
            r = read(fds[0], buffer, CGI_BUFFER - 1);
            body += buffer;
        }
    }

    close(fds[1]);
    close(fds[0]);

    for (size_t i = 0; i < _env.size(); i++)
        delete [] env[i];
    delete [] env;

    return body;
}
