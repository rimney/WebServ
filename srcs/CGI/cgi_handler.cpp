

# include "../../includes/cgi_handler.hpp"

cgi_handler::cgi_handler() {}

cgi_handler::cgi_handler(server_parser server_config, Request request)
    : _server_config(server_config), _request(request),
    _location(_server_config.getServerLocationsObject()[_request.get_start_line().location_index])
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
    // _env.push_back(std::string("SCRIPT_NAME=") + _location.getCgiPathObject());
    
    if (headers.find("Hostname") != headers.end())
        _env.push_back(std::string("SERVER_NAME=") + headers["Hostname"]);
    else
        _env.push_back(std::string("SERVER_NAME=") + std::to_string(_server_config.getHostObject()));
    
    _env.push_back(std::string("SERVER_PORT=") + std::to_string(_server_config.getPortObject()));
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("SERVER_SOFTWARE=WebServ/0.0");
    _env.push_back("REDIRECT_STATUS=200");

    // print all env variables
    // for  (std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    //     std::cout << *it << '\n';
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

void cgi_handler::exec(respond & response)
{
    pid_t       cgi_pid;
    int         fds[2];
    char**      env;
    std::string cgi_response;

    env = vector_to_ptr();

    pipe(fds);

    cgi_pid = fork();

    if (cgi_pid == -1)
    {
        std::cerr << "ERROR: fork() failed\n";
        response.setstatusCode("500");
        response.setstatusDescription("Internal Server Error");
        // add page error
        response.mergeRespondStrings();
        return ;
    }
    else if (cgi_pid == 0)
    {
        if (dup2(fds[1], STDOUT_FILENO) == -1)
        {
            std::cerr << "ERROR: dup2() failed\n";
            write(fds[1], "500", 3);
        }
        if (dup2(fds[0], STDIN_FILENO) == -1)
        {
            std::cerr << "ERROR: dup2() failed\n";
            write(fds[1], "500", 3);
        }

        // char * const argv[3] = {
        //     // (char *) _location.getCgiPathObject().c_str(),
        //     (char *) _request.get_start_line().full_path.c_str(),
        //     NULL
        // };

        // execve(_location.getCgiPathObject().c_str(), argv, env);

        std::cerr << "ERROR: execve() failed\n";
        write(fds[1], "500", 3);
    }
    else
    {
        char    buffer[CGI_BUFFER] = {0};

        waitpid(-1, NULL, 0);
        fcntl(fds[0], F_SETFL, O_NONBLOCK);
        while (1)
        {
            memset(buffer, 0, CGI_BUFFER);
            if (read(fds[0], buffer, CGI_BUFFER - 1) <= 0)
                break ;
            cgi_response.append(buffer);
        }
    }

    close(fds[1]);
    close(fds[0]);

    for (size_t i = 0; i < _env.size(); i++)
        delete [] env[i];
    delete [] env;
    if (cgi_pid == 0)
        exit(0);

    generate_response(cgi_response, response);
}


void    cgi_handler::generate_response(std::string & cgi_response, respond & response)
{
    size_t      pos = 0;
    size_t      i = 0;
    size_t      j;
    std::string header, element;
    size_t      content_length;

    if (cgi_response.find("500") != std::string::npos || cgi_response.empty())
    {
        response.setstatusCode("500");
        response.setstatusDescription("Internal Server Error");
        // add page error
        response.mergeRespondStrings();
        return ;
    }

    while ((pos = cgi_response.find("\r\n", i)) != std::string::npos)
    {
        if (pos == i)
            break ;
        
        header = cgi_response.substr(i, pos - i);

        if ((j = header.find(':')) != std::string::npos)
        {
            element = header.substr(0, j);
            if (element == "Content-Length")
                response.setContentLenght(header.substr(j));
            else if (element == "Content-type")
                response.setContentType(header.substr(j));
        }
        i += header.size() + 2;
        element.clear();
        header.clear();
    }

    if (pos == std::string::npos)
    {
        response.setBody(cgi_response);
        response.setContentType("text/html");
    }
    else
        response.setBody(cgi_response.substr(pos + 2));

    std::stringstream ss(response.getContentLenght());
    ss >> content_length;

    if (content_length == 0 && !response.getBody().empty())
        response.setContentLenght(std::to_string(response.getBody().size()));

    response.mergeRespondStrings();
}