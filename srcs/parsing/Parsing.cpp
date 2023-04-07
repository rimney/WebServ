/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 03:50:36 by rimney            #+#    #+#             */
/*   Updated: 2023/04/07 06:16:30 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"
#include <arpa/inet.h>


char *toIp(int ip)
{
    struct in_addr addr;
    addr.s_addr = htonl((u_int32_t)ip);
    char* ip_address = inet_ntoa(addr);

    return(ip_address);
}

std::string fileExist(std::string root, std::string file)
{
    std::string fileHolder;
    std::ifstream init(file);


    if(init.good())
        return (file);
    else
    {
        fileHolder = root + file;
        std::ifstream file(fileHolder);
        if (file.good())
            return (fileHolder);
        else
        {
            return ("Error");
        }
    }
    return ("Error");
}


server_location::server_location(server_location const  & s)
{

    this->location_index = s.location_index;
    this->location_name = s.location_name;
    this->root = s.root;
    this->HttpMethods = s.HttpMethods;
    this->index = s.index;
    this->cgiPaths = s.cgiPaths;
    this->cgiExt = s.cgiExt;
    this->error_codes = s.error_codes;
    this->error_page = s.error_page;
    this->client_max_body_size = s.client_max_body_size;
    this->is_auto_index = s.is_auto_index;
    this->has_cgi = s.has_cgi;
    this->has_redirection = s.has_redirection;
    this->redirection = s.redirection;
    this->upload = s.upload;
    this->has_301_code = s.has_301_code;
}
server_location & server_location::operator=(server_location const & s)
{
    this->location_index = s.location_index;
    this->location_name = s.location_name;
    this->root = s.root;
    this->HttpMethods = s.HttpMethods;
    this->index = s.index;
    this->cgiPaths = s.cgiPaths;
    this->cgiExt = s.cgiExt;
    this->error_codes = s.error_codes;
    this->error_page = s.error_page;
    this->client_max_body_size = s.client_max_body_size;
    this->has_cgi = s.has_cgi;
    this->is_auto_index = s.is_auto_index;
    this->has_redirection = s.has_redirection;
    this->redirection = s.redirection;
    this->upload = s.upload;
    this->has_301_code = s.has_301_code;
    return (*this);
}

bool server_location::isCgi(std::string path)
{
    char  const *s = strrchr(path.c_str(), '.');
    if(!s)
        return (false);
    if(!strcmp(strrchr(path.c_str(), '.'), ".php") || !strcmp(strrchr(path.c_str(), '.'), ".rb") || !strcmp(strrchr(path.c_str(), '.'), ".py"))
        return (true);
    return (false);
}

void    server_location::getErrorPage(std::string *keys, size_t size)
{
    if (size <= 1)
    {
        std::cerr << "Error Error Page Not Found\n";
        exit(1);
    }
    this->error_page = keys[size - 1];
    for(size_t i = 1; i < size - 1;i++)
        this->error_codes.push_back(stoi(keys[i]));
    delete [] keys;
}
void    server_location::getIndexPage(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Missing index page !";
        exit(1);
    }
    this->index = keys[size - 1];
    delete [] keys;
}
void    server_location::getAutoIndex(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Autoindex Bad Argument\n";
        exit(1);
    }
    if(keys[size - 1] == "on")
        this->is_auto_index = true;
    else if(keys[size - 1] == "off")
        this->is_auto_index = false;
    else
    {
        std::cerr << "Error Wrong autoindex Argument";
        exit(1);
    }
    delete [] keys;
}
void    server_location::getRoot(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Missing Root Page\n";
        exit(1);
    }
    this->root = keys[size - 1];
    delete [] keys;
}

void    server_location::getRedirection(std::string *keys, size_t size)
{
    if(size != 3)
    {
        std::cerr << "Error Redirection Arguments !\n";
        exit(1);
    }
    if(size == 3 && keys[1] == "301")
        this->has_301_code = true;
    else if (size == 3 && keys[1] != "301")
    {
        std::cerr << "Error : Check Redirection Code Value\n";
        exit(1);
    }
    this->redirection = keys[size - 1];

    delete [] keys;
}

void    server_location::getLocationCmds(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Cmbs Arguments";
        exit(1);
    }
    this->client_max_body_size = stoi(keys[size - 1]);
    delete [] keys;
}
void    server_location::getMethods(std::string *Keys, size_t size)
{
    if(size <= 1 || size > 4)
    {
        std::cerr << "Error Methods Assignemt\n";
        exit(1);
    }
    for(size_t i = 1;i < size; i++)
    {
        Keys[i].erase(Keys[i].find_last_not_of(",") + 1);
        this->HttpMethods.push_back(Keys[i]);
    }
    delete [] Keys;
}

void    server_location::getCgiPath(std::string *Keys, size_t size)
{
    if(size <= 1)
    {
        std::cerr << "Error Cgi Path Assignemt\n";
        exit(1);
    }
    for(size_t i = 1; i < size; i++)
        this->cgiPaths.push_back(Keys[i]);

    delete [] Keys;
}


void    server_location::getCgiExec(std::string *Keys, size_t size)
{
    if(size <= 1)
    {
        std::cerr << "Error Cgi Exec Assinment\n";
        exit(1);
    }
    for(size_t i = 1; i < size; i++)
        this->cgiExt.push_back(Keys[i]);
    delete[] Keys;
}

void    server_location::getLocationName(std::string *Keys, size_t size)
{
    if(size <= 1 || size > 3)
    {
        std::cerr << "Error Location Name Arguments\n";
        exit(1);
    }
    if(Keys[size - 1] != "{")
        this->location_name = Keys[size - 1];
    else
        this->location_name = Keys[size - 2];
    
    delete [] Keys;
}

void    server_location::getUpload(std::string *Keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Upload Arguments\n";
        exit(1);
    }
    this->upload = Keys[size - 1];
    delete [] Keys; 
}

void    server_location::checkCgiAllowed(void)
{
    bool f = false;
    std::string path_holder;
    std::string ext_holder;

    for(size_t i = 0; i < this->cgiExt.size(); i++)
    {
        if(this->cgiExt[i] != ".php" && this->cgiExt[i] != ".rb" && this->cgiExt[i] != ".py")
        {
            std::cerr << "Error : CGI Extention Not Allowed\n";
            exit(1);
        }
    }
    for(size_t i = 0; i < this->cgiPaths.size(); i++)
    {
        f = false;
        if(isFileOrDirectory(this->cgiPaths[i]) != "file")
        {
            std::cerr << "Error : " << this->cgiPaths[i] << " Check This Path\n";
            exit(1);
        }
        if(!strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "php") || !strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "php-cgi"))
        {
            for(size_t i = 0; i < this->cgiExt.size(); i++)
            {
                if(this->cgiExt[i] == ".php")
                    f = true;
            }
            if(f == false)
            {
                std::cerr << "Error : There's No Extention Of : " << this->cgiPaths[i] << std::endl;
                exit(1);
            }
        }
        else if(!strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "python") || !strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "python3"))
        {
            for(size_t i = 0; i < this->cgiExt.size(); i++)
            {
                if(this->cgiExt[i] == ".py")
                    f = true;
            }
            if(f == false)
            {
                std::cerr << "Error : There's No Extention Of : " << this->cgiPaths[i] << std::endl;
                exit(1);
            }
        }
        else if(!strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "ruby"))
        {
            for(size_t i = 0; i < this->cgiExt.size(); i++)
            {
                if(this->cgiExt[i] == ".rb")
                    f = true;
            }
            if(f == false)
            {
                std::cerr << "Error : There's No Extention Of : " << this->cgiPaths[i] << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cerr << "Error : Cgi Not Supported\n";
            exit(1);
        }
    }
}

std::string server_location::getCgiPathObject(std::string path)
{
    if(strrchr(path.c_str(), '.') == NULL)
        return "";
    for(size_t i = 0; i < this->cgiPaths.size(); i++)
    {
        if((!strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "php") || !strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "php-cgi")) && !strcmp(strrchr(path.c_str(), '.') + 1, "php"))
            return (this->cgiPaths[i]);
        if((!strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "python") || !strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "python3")) && !strcmp(strrchr(path.c_str(), '.') + 1, "py"))
            return (this->cgiPaths[i]);
        if(!strcmp(strrchr(this->cgiPaths[i].c_str(), '/') + 1, "ruby") && !strcmp(strrchr(path.c_str(), '.') + 1, "py"))
            return (this->cgiPaths[i]);
    }
    return "";
}
void server_location::construct_location(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{
    std::vector<std::string> locationVec(first, last);
    size_t temp_size;

    this->has_cgi = false;
    this->is_auto_index = false;
    this->has_redirection = false;
    this->client_max_body_size = 0;
    for(size_t i = 0; i < locationVec.size(); i++)
    {
        if ((!strncmp(locationVec[i].c_str(), "location", 8) && locationVec[i].back() == '{') || (!strncmp(locationVec[i].c_str(), "location", 8) && locationVec[i + 1] == "{"))
        {
            getLocationName(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if(!strncmp(locationVec[i].c_str(), "error", 5))
        {
            getErrorPage(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
            // if(error_codes.size() == 0)
            //     error_codes.push_back(404);
            if(isFileOrDirectory(this->getLocationErrorPageObject()) == "error")
            {
                std::cerr << "Error : Check The Location Error Path\n";
                exit(1);
            }
        }
        else if(!strncmp(locationVec[i].c_str(), "index", 5))
        {
            getIndexPage(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if(!strncmp(locationVec[i].c_str(), "root", 4))
        {
            if(locationVec[i][locationVec[i].size() - 1] == '/')
                locationVec[i].erase(locationVec[i].size() - 1);
            getRoot(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
            if(isFileOrDirectory(this->getLocationRootObject()) == "error")
            {
                std::cerr << "Error : Check The Location Root Path\n";
                exit(1);
            }
        }
        else if (!strncmp(locationVec[i].c_str(), "autoindex", 9))
        {
            getAutoIndex(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "return ", 7))
        {
            getRedirection(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
            this->has_redirection = true;
        }
        else if (!strncmp(locationVec[i].c_str(), "client_max_body_size", 19))
        {
            getLocationCmds(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "methods", 7))
        {
            getMethods(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "cgi_path ", 9))
        {
            this->has_cgi = true;
            getCgiPath(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
 
            checkCgiAllowed();
        }
        else if (!strncmp(locationVec[i].c_str(), "cgi_exec ", 9))
        {
            getCgiExec(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "upload ", 7))
        {
            getUpload(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if(locationVec[i] != "{" && locationVec[i] != "}" && locationVec[i][0] != '#' && !locationVec[i].empty())
        {
            std::cerr << "Error : Wrong Directive [" << locationVec[i] << "]\n";
            exit(1);
        }
    }
}
std::string *stringSplit(std::string split, char c, size_t *index_save)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = 0;
    while ((end = split.find(c, start)) != std::string::npos)
    {
        tokens.push_back(split.substr(start, end - start));
        start = end + 1;
        while (start < split.length() && (split[start] == c || isspace(split[start]))) {
            ++start;
        }
    }
    tokens.push_back(split.substr(start));
    std::string *ret = new std::string[tokens.size()]; // ret string allocation;
    std::copy(tokens.begin(), tokens.end(), ret);
    for(size_t i = 0; i < tokens.size(); i++)
    {
        ret[i].erase(0, ret[i].find_first_not_of(" \t\r\n"));
        ret[i].erase(ret[i].find_last_not_of(" \t\r\n") + 1);
    }
    *index_save = tokens.size();
    return (ret);
}

server_location server_parser::getOneLocationObject(int index) const
{
    return (this->location[index]);
}


server_parser::server_parser(server_parser const & s) : server_location(s)
{
    this->port = s.getPortObject();
    this->host = s.getHostObject();
    this->server_index = s.getServerIndexObject();
    this->location_count = s.getLocationCount();
    this->client_max_body_size = s.getCmbsObject();
    this->redirection = s.getRedirectionObject();
    this->is_auto_index = s.getIsAutoIndexObject();
    this->root = s.getRootObject();
    this->server_names = s.getServerNamesObject();
    this->error_page = s.getServerErrorPageObject();
    this->location = s.getServerLocation();
    this->error_codes = s.getErrorCodesObject();
}

server_parser & server_parser::operator=(server_parser const & s)
{
    this->port = s.getPortObject();
    this->host = s.getHostObject();
    this->server_index = s.getServerIndexObject();
    this->location_count = s.getLocationCount();
    this->client_max_body_size = s.getCmbsObject();
    this->redirection = s.getRedirectionObject();
    this->is_auto_index = s.getIsAutoIndexObject();
    this->root = s.getRootObject();
    this->server_names = s.getServerNamesObject();
    this->error_page = s.getServerErrorPageObject();
    this->error_codes = s.getErrorCodesObject();
    this->location = s.getServerLocationsObject();
    return (*this);
}
std::vector<server_location> server_parser::getServerLocationsObject(void) const
{
    return (this->location);
}
std::vector<std::string> server_parser::getServerNamesObject(void) const
{
    return (this->server_names);
}
std::vector<int> server_parser::getPortObject(void) const
{
    return (this->port);
}
int server_parser::getHostObject(void) const
{
    return (this->host);
}
int     server_location::getLocationCmbsObject(void)
{
    return (this->client_max_body_size);
}
int server_parser::getServer_IndexLocationObject(void) const
{
    return (this->location_index);
}
size_t server_parser::getLocationCount(void) const
{
    return (this->location.size());
}
int server_parser::getCmbsObject(void) const
{
    return (this->client_max_body_size);
}
std::vector<int> server_parser::getErrorCodesObject(void) const
{
    return (this->error_codes);
}
std::string server_parser::getIndexObject(void) const
{
    return (this->index);
}
std::string server_parser::getRootObject(void) const
{
    return (this->root);
}
std::string server_parser::getRedirectionObject(void) const
{
    return (this->redirection);
}
std::vector<server_location> server_parser::getServerLocation(void) const
{
    return (this->location);
}
bool server_parser::getIsAutoIndexObject(void) const
{
    return (this->is_auto_index);
}
int server_parser::getServerLocationCountObject(void) const
{
    return (this->location_count);
}
int server_parser::getServerIndexObject(void) const
{
    return (this->server_index);
}
std::string server_parser::getServerErrorPageObject(void) const
{
    return (this->error_page);
}
/////// GETTERS AND SETTERS /////////////
bool server_parser::is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

int server_parser::ipToInt(std::string host)
{
    size_t size;
    int ret = 0; // we can make it size_t !!
    std::vector<int> Array;
    std::string *keys = stringSplit(host, '.', &size);
    for(size_t i = 0; i < size; i++)
    {
        Array.push_back(stoi(keys[i]));
        if(!is_digits(keys[i]) || Array[i] > 256 || Array[i] < 0)
        {
            std::cerr << "Error Wrong IP Value\n";
            exit(1);
        }
    }
    if(size != 4)
    {
        std::cerr << "Wrong ip Address";
        exit(1);
    }
    for (int i = 0; i < 4; i++) {
        ret |= (Array[i] << (8 * (3 - i)));
    }
    delete [] keys;
    return (ret);
}
void    server_parser::getPort(std::string *Port, size_t temp_size)
{
    if(temp_size < 1)
    {
        std::cerr << "Error : Check The Port Arguments\n";
        exit(1);
    }
    for(size_t i = 1; i < temp_size; i++)
    {
        if(atoi(Port[i].c_str()) < 1)
        {
            delete [] Port;
            std::cerr << "Error : Wrong Port Value\n";
            exit(1);
        }
        this->port.push_back(atoi(Port[i].c_str()));
    }
    delete [] Port;
}
void    server_parser::getServerName(std::string *keys, size_t size)
{
    if (size <= 1)
    {
        std::cerr << "Error Missing Server Name !";
        exit(1);
    }
    if(this->server_names.size() > 0)
    {
        std::cerr << "Error Duplicate Server_name !";
        exit(1);
    }
    for(size_t i = 1; i < size; i++)
        this->server_names.push_back(keys[i]);

    delete [] keys;
}
void    server_parser::getErrorPage(std::string *keys, size_t size)
{
    if (size <= 1)
    {
        std::cerr << "Error Error Page Not Found\n";
        exit(1);
    }
    this->error_page = keys[size - 1];
    for(size_t i = 1; i < size - 1;i++)
    {
        if(!(stoi(keys[i]) >= 400 && stoi(keys[i]) <= 501))
        {
            std::cerr << "Error : Wrong Error Code Range !\n";
            delete [] keys;
            exit(1);
        }
        this->error_codes.push_back(stoi(keys[i]));
    }
    delete [] keys;
}
void    server_parser::getIndexPage(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Missing index page !";
        exit(1);
    }
    this->index = keys[size - 1];
    delete [] keys;
}

void    server_parser::getAutoIndex(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Autoindex Bad Argument\n";
        exit(1);
    }
    if(keys[size - 1] == "on")
        this->is_auto_index = true;
    else if(keys[size - 1] == "off")
        this->is_auto_index = false;
    else
    {
        std::cerr << "Error Wrong autoindex Argument";
        exit(1);
    }
    delete [] keys;
}

void    server_parser::getRoot(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Missing Root Page\n";
        exit(1);
    }
    this->root = keys[size - 1];
    delete [] keys;
}

void    server_parser::getRedirection(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Redirection Arguments !\n";
        exit(1);
    }
    this->redirection = keys[size - 1];
    delete [] keys;
}

void    server_parser::getCmds(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cerr << "Error Cmbs Arguments";
        exit(1);
    }
    this->client_max_body_size = stoi(keys[size - 1]);
    delete [] keys;
}

int server_parser::getLocationByName(std::string name) const
{
    for(size_t i = 0; i < location.size(); i++)
    {
        if(location[i].getLocationNameObject() == name)
            return (i);
    }
    return (-1);
}

void    server_parser::getHost(std::string *keys, size_t size)
{
    // this->host = ipToInt(temparray[0]);
    if(size < 1 || size > 2)
    {
        std::cerr << "Error : Check The Host Argumets";
        exit(1);
    }
    this->host = ipToInt(keys[1]);
    delete [] keys;
}

void    server_parser::checkServerData(void)
{
    if(this->port.size() == 0)
    {
        std::cerr << "Error : Server Missing Port\n";
        exit(1);
    }
    else if(this->getServerErrorPageObject().empty())
    {
        std::cerr << "Error : Server Missing Error Page\n";
        exit(1);
    }
    else if(this->getLocationByName("/") == -1)
    {
        std::cerr << "Error : Server Missing \\ Location\n";
        exit(1);
    }
    else if(this->host == -1)
    {
        std::cerr << "Error : Host Not Found\n";
        exit(1);
    }
}

void    server_parser::construct_server(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{
    this->is_auto_index = false;
    this->server_had_delete_method = true;
    this->server_has_get_method = true;
    this->server_has_post_method = true;
    // this->client_max_body_size = 0;
    this->host = -1;
    int location_index = 0;
    std::vector<std::string> serverVec(first, last);
    size_t opening_bracket = 0;
    size_t closing_bracket = 0;
    size_t temp_size;

    for(size_t i = 1; i < serverVec.size(); i++)
    {
        if(!strncmp(serverVec[i].c_str(), "host ", 5))
        {
            if(this->host != -1)
            {
                std::cerr << "Error : Duplicate Host !\n";
                exit(1);
            }
            getHost(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
            // getPort(stringSplit(serverVec[i], ' ', &temp_size), temp_size); // host and port parsing;
        }
        else if(!strncmp(serverVec[i].c_str(), "port ", 5))
        {
            getPort(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
        }
        else if(!strncmp(serverVec[i].c_str(), "server_name", 11))
        {
            getServerName(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
            if(this->server_names.size() == 0)
                this->server_names.push_back("localhost ");
        }
        else if(!strncmp(serverVec[i].c_str(), "error", 5))
        {
            getErrorPage(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
            if(isFileOrDirectory(this->getServerErrorPageObject()) == "error")
            {
                std::cerr << "Error : Check The Server Error Path\n";
                exit(1);
            }
        }
        else if(!strncmp(serverVec[i].c_str(), "index", 5))
        {
            getIndexPage(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
            if(isFileOrDirectory(this->getRootObject() + this->getIndexObject()) == "error")
            {
                std::cerr << "Error : Check The Server Index Path\n";
                exit(1);
            }
        }
        else if(!strncmp(serverVec[i].c_str(), "root", 4))
        {
            getRoot(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
            if(isFileOrDirectory(this->getRootObject()) == "error")
            {
                std::cerr << "Error : Check The Server Root Path\n";
                exit(1);
            }
        }
        else if (!strncmp(serverVec[i].c_str(), "autoindex", 9))
        {
            getAutoIndex(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(serverVec[i].c_str(), "return", 6))
        {
            getRedirection(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(serverVec[i].c_str(), "client_max_body_size", 19))
        {
            getCmds(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
        }
        else if ((!strncmp(serverVec[i].c_str(), "location", 8) && serverVec[i].back() == '{') || (!strncmp(serverVec[i].c_str(), "location", 8) && serverVec[i + 1] == "{"))
        {
            opening_bracket = i;
            i++;
            while(serverVec[i] != "}")
                i++;
            closing_bracket = i;
            server_location locationn;
            locationn.setLocationIndex(location_index);
            locationn.construct_location(serverVec.begin() + opening_bracket, serverVec.begin() + closing_bracket);   
            this->location.push_back(locationn);
            location_index++;          
        }
        else if(serverVec[i] != "{" && serverVec[i] != "}" && serverVec[i][0] != '#' && !serverVec[i].empty())
        {
            std::cerr << "Error : Wrong Directive [ " << serverVec[i] << " ]\n";
            exit(1);
        }
    }
    this->getServerDataFromRootLocation();
    this->checkServerData();
}

void    server_parser::setLocationsIndex(std::vector<server_location> location)
{
    for(size_t i = 0; i < location.size();i++)
        location[i].setLocationIndex(i);
}

void    server_parser::setServerIndex(int index)
{
    this->server_index = index;
}

size_t server_parser::getLocationCount(std::vector<std::string> vec)
{
    size_t count = 0;
    for(size_t i = 0; i < vec.size(); i++)
    {
        if (!strncmp(vec[i].c_str(), "location ", 9) && vec[i].back() == '{')
            count++;
    }
    return (count);
}

void    server_parser::restoreRootObject(int i)
{
    if(this->root.size() == 0 && this->location[i].getLocationRootObject().size() != 0)
    {
        this->root = this->location[i].getLocationRootObject();
    }
    else if(this->root.size() != 0 && this->location[i].getLocationRootObject().size() != 0)
    {
        std::cerr << "Error : Duplicate Root Path\n";
        exit(1);
    }
    else if(this->root.size() == 0 && this->location[i].getLocationRootObject().size() == 0)
    {
        std::cerr << "Error : There's No Root In Server\n";
        exit(1);   
    }
}

void    server_parser::restoreIndexObject(int i)
{
    if(this->index.size() == 0 && this->location[i].getLocationIndexObject().size() != 0)
    {
        this->index = this->location[i].getLocationIndexObject();
    }
    else if(this->index.size() != 0 && this->location[i].getLocationIndexObject().size() != 0)
    {
        std::cerr << "Error : Duplicate Index Path\n";
        exit(1);
    }
}

void    server_parser::restoreAutoIndex(int i)
{
    if(this->is_auto_index == false && this->location[i].getLocationIsAutoIndexObject() == true)
    {
        this->is_auto_index = true;
    }
    else if(this->is_auto_index == true && this->location[i].getLocationIsAutoIndexObject() == true)
    {
        std::cerr << "Error : There's Two True AutoIndex Bool !\n";
        exit(1);
    }
}

void    server_parser::restoreCmbs(int i)
{
    this->client_max_body_size = this->location[i].getLocationCmbsObject();
}

void    server_parser::getServerDataFromRootLocation(void)
{
    for(size_t i = 0; i < location.size(); i++)
    {
        if(this->location[i].getLocationNameObject() == "/")
        {
            this->restoreAutoIndex(i);
            this->restoreRootObject(i);
            this->restoreIndexObject(i);
            this->restoreCmbs(i);
        }
    }
}

config_parser::config_parser(config_parser & c)
{
    this->servers = c.getServersObject();
}

config_parser & config_parser::operator=(config_parser const & c)
{
    this->server_count = c.getServerCountObject();
    this->servers = c.getServersObject();
    return (*this);
}

/////// GETTERS AND SETTERS //////////
size_t config_parser::getServerCountObject(void) const
{
    return (this->server_count);
}

std::vector<server_parser> config_parser::getServersObject(void) const
{
    return (this->servers);
}

config_parser::config_parser(std::string filename)
{
    std::vector<std::string> tempConf;
    std::vector<server_parser> holder;

    std::ifstream file(filename);
    if(!file.good())
    {
        std::cerr << "Error : Cannot Open File!\n";
        exit(1);
    }
    checkFileBrackets(filename);
    std::string line;
    int         opening_bracket;
    int         closing_bracket;
    int         server_index = 0;
    while(std::getline(file, line))
    {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n;") + 1);
        std::replace(std::begin(line),std::end(line),'\t',' ');
        std::replace(std::begin(line),std::end(line),'\n',' ');
        tempConf.push_back(line);
    }
    file.close();
	for(std::vector<std::string>::size_type i = 0; i < tempConf.size(); i++)
	{
		if ((!strncmp(tempConf[i].c_str(), "server", 6) && tempConf[i].back() == '{') || (!strncmp(tempConf[i].c_str(), "server", 6) && tempConf[i + 1] == "{"))
		{
			opening_bracket = i;
			while(tempConf[i] != "}")
			{
				if ((!strncmp(tempConf[i].c_str(), "location", 8) && tempConf[i].back() == '{') || (!strncmp(tempConf[i].c_str(), "location", 8) && tempConf[i + 1] == "{"))
    			{
                	while(tempConf[i] != "}")
                    	i++;
            	}
                i++;
            }
            closing_bracket = i;
            server_parser server;
            server.construct_server(tempConf.begin() + opening_bracket, tempConf.begin() + closing_bracket + 1);
            server.setServerIndex(server_index);
            this->servers.push_back(server);
            server_index += 1;
        }
		else if(!tempConf[i].empty())
        {
            std::cerr << "Error : Wrong Directive [" << tempConf[i] << "]\n";
            exit(1);
        }
    }
    if(hasDuplicatePorts())
    {
    	std::cerr << "Error : Server Has Duplicate Ports\n";
    	exit(1);
    }
}

bool    config_parser::hasDuplicatePorts()
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        const server_parser & server1 = servers[i];
        for (size_t j = i + 1; j < servers.size(); j++)
        {
            const server_parser & server2 = servers[j];
            for (size_t k = 0; k < server1.getPortObject().size(); k++)
            {
                int port = server1.getPortObject()[k];
                for (size_t l = 0; l < server2.getPortObject().size(); l++)
                {
                    int port2 =  server2.getPortObject()[l];
                    if (port == port2)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void    config_parser::checkFileBrackets(std::string f)
{
    std::ifstream file(f);
    std::stack<char> brackets;
    char c;
    int i = 0;
    bool balanced = true;

    while (file.get(c))
	{
        ++i;
        if (c == '(' || c == '{' || c == '[')
            brackets.push(c);
		else if (c == ')' || c == '}' || c == ']')
		{
            if (brackets.empty())
			{
                balanced = false;
                std::cerr << "Unclosed Bracket !\n";
                exit(1);
            }
            char top = brackets.top();
            if ((c == ')' && top == '(') || (c == '}' && top == '{') || (c == ']' && top == '['))
                brackets.pop();
			else
			{
                balanced = false;
                std::cerr << "Unclosed Bracket !\n";
                exit(1);
            }
        }
    }
}

void    config_parser::servers_index_init()
{
    for(size_t i = 0;i < this->servers.size(); i++)
        this->servers[i].setServerIndex(i);
}

int    config_parser::getServersCount(std::vector<std::string> vec)
{
    int count = 0;
    for(std::vector<std::string>::size_type i = 0; i < vec.size(); i++)
        {
            if ((!strncmp(vec[i].c_str(), "server", 6) && vec[i].back() == '{') || (!strncmp(vec[i].c_str(), "server", 6) && vec[i + 1] == "{"))
                count++;  
        }
        return (count);
}
