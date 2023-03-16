/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 03:50:36 by rimney            #+#    #+#             */
/*   Updated: 2023/03/16 12:49:25 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.hpp"
#include <arpa/inet.h>

char *toIp(int ip)
{
    struct in_addr addr;
    addr.s_addr = htonl((u_int32_t)ip);
    char* ip_address = inet_ntoa(addr);

    return(ip_address);
}

std::string fileExist(server_parser server, std::string root, std::string file)
{
    std::string fileHolder;
    std::ifstream init(file);


    if(init.good())
        return (file);
    else
    {
        std::string s = "/";
        fileHolder = root + s + file;
        std::ifstream file(fileHolder);
        if (file.good())
            return (fileHolder);
        else
        {
            return (server.getServerErrorPageObject());
        }
    }
    return (server.getServerErrorPageObject());
}

std::ostream & operator<<(std::ostream & os, server_location & s)
{
    std::vector<int> locationErrorCodes = s.getLocationErrorCodesObject();
    std::vector<std::string> locationMethods = s.getLocationMethodsObject();
    os << "     |------------->>\n";
    os << "     | Location index : " << s.getLocationindexObject() << '\n';
    os << "     | Location  Name : " << s.getLocationNameObject() << '\n';
    os << "     | Location CMBS : " << s.getLocationCmbsObject() << '\n';
    for(size_t i = 0;i < locationErrorCodes.size();i++)
        os << "     | Location Error codes : " << locationErrorCodes[i] << '\n';
    os << "     | Location Error Page : " << s.getLocationErrorPageObject() <<  '\n';
    os << "     | Location Root : " << s.getLocationRootObject() << '\n';
    os << "     | Location Index Page : " << s.getLocationIndexObject() << '\n';
    os << "     | Location Redirection : " << s.getLocationRedirectionObject() << '\n';
    for(size_t i = 0; i < locationMethods.size(); i++)
        os << "     | Location Methods : "<< locationMethods[i] << '\n';
        os << "     | Location AutoIndex : " << s.getLocationIsAutoIndexObject() << '\n'; 
        os << "     |------------->>\n";
        return (os);
}

std::ostream & operator<<(std::ostream & os, server_parser & s)
{
    std::vector<int> serverErrorCodes = s.getErrorCodesObject();
    std::vector<std::string> serverNames = s.getServerNamesObject();
    std::vector<server_location> serverLocations = s.getServerLocationsObject();
    os << "|------------->>\n";
    os << "| Server index : " << s.getServerIndexObject() << '\n';
    os << "| Server Port : " << s.getPortObject() << '\n';
    os << "| Server Host : " << s.getHostObject() << " ( " << toIp(s.getHostObject())<<  " )" <<'\n';
    os << "| Server Location Count: " << s.getServerLocationCountObject() << '\n';
    os << "| Server CMBS : " << s.getCmbsObject() << '\n';
    for(size_t i = 0;i < serverErrorCodes.size();i++)
        os << "| Server Error codes : " << serverErrorCodes[i] << '\n';
    os << "| Server Error Page : " << s.getServerErrorPageObject() <<  '\n';
    os << "| Server Root : " << s.getRootObject() << '\n';
    os << "| Server Index Page : " << s.getIndexObject() << '\n';
    os << "| Server Redirection : " << s.getRedirectionObject() << '\n';
    for(size_t i = 0; i < serverNames.size(); i++)
        os << "| Server Names : "<< serverNames[i] << '\n';
        os << "| Server AutoIndex : " << s.getIsAutoIndexObject() << '\n';
    for(size_t i = 0; i < serverLocations.size(); i++)
        os << serverLocations[i] << '\n';
    os << "|------------->>\n";
    return (os);
}

std::ostream& operator<<(std::ostream& os, config_parser& p)
{
    std::vector<server_parser>  servers = p.getServersObject();
    for(size_t i = 0; i < servers.size(); i++)
        os << servers[i] << '\n';
    return (os);
}

server_location::server_location(server_location const  & s)
{

    this->location_index = s.location_index;
    this->location_name = s.location_name;
    this->root = s.root;
    this->is_auto_index = s.is_auto_index;
    this->HttpMethods = s.HttpMethods;
    this->index = s.index;
    this->cgiPath = s.cgiPath;
    this->cgiExt = s.cgiExt;
    this->error_codes = s.error_codes;
    this->error_page = s.error_page;
    this->client_max_body_size = s.client_max_body_size;
    // exit();
}
server_location server_location::operator=(server_location const & s)
{
    this->location_index = s.location_index;
    this->location_name = s.location_name;
    this->root = s.root;
    this->is_auto_index = s.is_auto_index;
    this->HttpMethods = s.HttpMethods;
    this->index = s.index;
    this->cgiPath = s.cgiPath;
    this->cgiExt = s.cgiExt;
    this->error_codes = s.error_codes;
    this->error_page = s.error_page;
    
    return (*this);
}
void    server_location::getErrorPage(std::string *keys, size_t size)
{
    if (size <= 1)
    {
        std::cout << "Error Error Page Not Found\n";
        exit(0);
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
        std::cout << "Error Missing index page !";
        exit(0);
    }
    this->index = keys[size - 1];
    delete [] keys;
}
void    server_location::getAutoIndex(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Autoindex Bad Argument\n";
        exit(0);
    }
    if(keys[size - 1] == "on")
        this->is_auto_index = true;
    else if(keys[size - 1] == "off")
        this->is_auto_index = false;
    else
    {
        std::cout << "Error Wrong autoindex Argument";
        exit(0);
    }
}
void    server_location::getRoot(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Missing Root Page\n";
        exit(0);
    }
    this->root = keys[size - 1];
    delete [] keys;
}
void    server_location::getRedirection(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Redirection Arguments !\n";
        exit(0);
    }
    this->redirection = keys[size - 1];
    delete [] keys;
}
void    server_location::getCmds(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Cmbs Arguments";
        exit(0);
    }
    this->client_max_body_size = stoi(keys[size - 1]);
    delete [] keys;
}
void    server_location::getMethods(std::string *Keys, size_t size)
{
    if(size <= 1 || size > 4)
    {
        std::cout << "Error Methods Assignemt\n";
        exit(0);
    }
    for(size_t i = 1;i < size; i++)
    {
        Keys[i].erase(Keys[i].find_last_not_of(",") + 1);
        this->HttpMethods.push_back(Keys[i]);
    }
    delete [] Keys;
}
void    server_location::getLocationName(std::string *Keys, size_t size)
{
    if(size <= 1 || size > 3)
    {
        std::cout << "Error Location Name Arguments";
        exit(0);
    }
    this->location_name = Keys[size - 2];
    delete [] Keys;
}
void server_location::construct_location(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{
    std::vector<std::string> locationVec(first, last);
    size_t temp_size;
    for(size_t i = 0; i < locationVec.size(); i++)
    {
        this->is_auto_index = false;
        this->client_max_body_size = 0;
        if(!strncmp(locationVec[i].c_str(), "location", 8) && locationVec[i].back() == '{')
        {
            getLocationName(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if(!strncmp(locationVec[i].c_str(), "error", 5))
        {
            getErrorPage(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
            if(error_codes.size() == 0)
                error_codes.push_back(404);
        }
        else if(!strncmp(locationVec[i].c_str(), "index", 5))
        {
            getIndexPage(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if(!strncmp(locationVec[i].c_str(), "root", 4))
        {
            getRoot(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "autoindex", 9))
        {
            getAutoIndex(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "return", 6))
        {
            getRedirection(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "client_max_body_size", 19))
        {
            getCmds(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
        }
        else if (!strncmp(locationVec[i].c_str(), "methods", 7))
        {
            getMethods(stringSplit(locationVec[i], ' ', &temp_size), temp_size);
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




server_parser::server_parser(server_parser const & s)
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
int server_parser::getPortObject(void) const
{
    return (this->port);
}
int server_parser::getHostObject(void) const
{
    return (this->host);
}
int     server_location::getLocationCmbsObject(void) const
{
    
    return (this->client_max_body_size);
}
int server_parser::getServer_IndexLocationObject(void) const
{
    return (this->location_index);
}
size_t server_parser::getLocationCount(void) const
{
    return (this->location_count);
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
            std::cout << "Error Wrong IP Value\n";
            exit(0);
        }
    }
    if(size != 4)
    {
        std::cout << "Wrong ip Address";
        exit(0);
    }
    for (int i = 0; i < 4; i++) {
        ret |= (Array[i] << (8 * (3 - i)));
    }
    delete [] keys;
    return (ret);
}
void    server_parser::getPort(std::string *Port, size_t temp_size)
{
    std::string *temparray;
    size_t temp_sizee;
    size_t pos;

    if(temp_size > 2)
    {
        std::cout << "Error listen has more than one argument !\n";
        exit(0);
    }
    pos = Port[1].find('.');
    if(!strncmp(Port[1].c_str(), "localhost:", 10) || pos != Port[1].npos)
    {
        temparray = stringSplit(Port[1], ':', &temp_sizee);
        if (temp_sizee > 2)
        {
            std::cout << "Error port has more than one location\n";
            exit(0);
        }
        this->port = stoi(temparray[1]);
        if(strncmp(Port[1].c_str(), "localhost:", 10)) 
            this->host = ipToInt(temparray[0]);
        delete [] temparray;
    }
    else if (temp_size == 2 && is_digits(Port[1]))
    {
        this->port = stoi(Port[1]);
    }
    delete [] Port;
}
void    server_parser::getServerName(std::string *keys, size_t size)
{
    if (size <= 1)
    {
        std::cout << "Error Missing Server Name !";
        exit(0);
    }
    if(this->server_names.size() > 0)
    {
        std::cout << "Error Duplicate Server_name !";
        exit(0);
    }
    for(size_t i = 1; i < size; i++)
        this->server_names.push_back(keys[i]);

    delete [] keys;
}
void    server_parser::getErrorPage(std::string *keys, size_t size)
{
    if (size <= 1)
    {
        std::cout << "Error Error Page Not Found\n";
        exit(0);
    }
    this->error_page = keys[size - 1];
    for(size_t i = 1; i < size - 1;i++)
        this->error_codes.push_back(stoi(keys[i]));
    delete [] keys;
}
void    server_parser::getIndexPage(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Missing index page !";
        exit(0);
    }
    this->index = keys[size - 1];
    delete [] keys;
    // exit(9);
}
void    server_parser::getAutoIndex(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Autoindex Bad Argument\n";
        exit(0);
    }
    if(keys[size - 1] == "on")
        this->is_auto_index = true;
    else if(keys[size - 1] == "off")
        this->is_auto_index = false;
    else
    {
        std::cout << "Error Wrong autoindex Argument";
        exit(0);
    }
    delete [] keys;
}
void    server_parser::getRoot(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Missing Root Page\n";
        exit(0);
    }
    this->root = keys[size - 1];
    delete [] keys;
}
void    server_parser::getRedirection(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Redirection Arguments !\n";
        exit(0);
    }
    this->redirection = keys[size - 1];
    delete [] keys;
}
void    server_parser::getCmds(std::string *keys, size_t size)
{
    if(size <= 1 || size > 2)
    {
        std::cout << "Error Cmbs Arguments";
        exit(0);
    }
    this->client_max_body_size = stoi(keys[size - 1]);
    delete [] keys;
}
void    server_parser::construct_server(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{
    this->port = 8080;
    this->is_auto_index = false;
    this->client_max_body_size = 0;
    this->host = 0;
    int location_index = 0;
    std::vector<std::string> serverVec(first, last);
    size_t opening_bracket = 0;
    size_t closing_bracket = 0;
    size_t temp_size;

    for(size_t i = 1; i < serverVec.size(); i++)
    {
        if(!strncmp(serverVec[i].c_str(), "listen ", 7))
        {
            getPort(stringSplit(serverVec[i], ' ', &temp_size), temp_size); // host and port parsing;
            if(this->host == 0)
                host = 2130706433;

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
            if(error_codes.size() == 0)
                error_codes.push_back(404);
        }
        else if(!strncmp(serverVec[i].c_str(), "index", 5))
        {
            getIndexPage(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
                
        }
        else if(!strncmp(serverVec[i].c_str(), "root", 4))
        {
            getRoot(stringSplit(serverVec[i], ' ', &temp_size), temp_size);
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
        else if (!strncmp(serverVec[i].c_str(), "location ", 9) && serverVec[i].back() == '{')
        {
            opening_bracket = i;
            i++;
            while(serverVec[i] != "}")
            {
                i++;
            }
            closing_bracket = i;
            server_location locationn;
            locationn.setLocationIndex(location_index);
            locationn.construct_location(serverVec.begin() + opening_bracket, serverVec.begin() + closing_bracket);   
            this->location.push_back(locationn);
            location_index++;          
        }
    }
        this->getServerDataFromRootLocation();


}
void    server_parser::setLocationsIndex(std::vector<server_location> location)
{
    for(size_t i = 0; i < location.size();i++)
        location[i].setLocationIndex(i);
}
void    server_parser::setServerIndex(int index)
{
    // std::cout << "CALLED\n";
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
    // std::cout << this->location[i] ;

    if(this->root.size() == 0 && this->location[i].getLocationRootObject().size() != 0)
    {
        std::cout << "root restored\n";
        this->root = this->location[i].getLocationRootObject();
    }
    else if(this->root.size() != 0 && this->location[i].getLocationRootObject().size() != 0)
    {
        std::cout << this->root << " this root <<\n";
        std::cerr << "Error : Duplicate Root Path\n";
        exit(0);
    }
    else if(this->root.size() == 0 && this->location[i].getLocationRootObject().size() == 0)
    {
        std::cerr << "Error : There's No Root In Server\n";
        exit(0);   
    }
    std::cout << "Root : " << this->root << '\n';
}

void    server_parser::restoreIndexObject(int i)
{
    if(this->index.size() == 0 && this->location[i].getLocationIndexObject().size() != 0)
    {
        std::cout << "index restored\n";
        this->index = this->location[i].getLocationIndexObject();
    }
    else if(this->index.size() != 0 && this->location[i].getLocationIndexObject().size() != 0)
    {
        std::cerr << "Error : Duplicate Index Path\n";
        exit(0);
    }
    else if(this->index.size() == 0 && this->location[i].getLocationIndexObject().size() == 0)
    {
        std::cerr << "Error : There's No Index In Server\n";
        exit(0);   
    }
    std::cout << "Index : " << this->index << '\n';
}

void    server_parser::getServerDataFromRootLocation(void)
{
    for(size_t i = 0; i < location.size(); i++)
    {
        if(this->location[i].getLocationNameObject() == "/")
        {
            this->restoreRootObject(i);
            this->restoreIndexObject(i);
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
                if (!strncmp(tempConf[i].c_str(), "location", 8) && tempConf[i].back() == '{')
                {
                    while(tempConf[i] != "}")
                        i++;
                }
                i++;
            }
            closing_bracket = i;
            server_parser server;
            server.setServerIndex(server_index);
            server.construct_server(tempConf.begin() + opening_bracket, tempConf.begin() + closing_bracket + 1);
            this->servers.push_back(server);
            server_index += 1;
        }
        
    }  
    for(size_t i = 0; i < servers.size(); i++)
        std::cout << servers[i];
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
