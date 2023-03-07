/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:53:42 by rimney            #+#    #+#             */
/*   Updated: 2023/03/06 23:53:41 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

class server_location
{
    protected :
        int     location_index;
        std::string location_name; // /location/example
        std::string root; // root key
        bool is_auto_index; // is autoindex
        std::vector<std::string>  HttpMethods; // GET, POST, DELETE
        
        std::string index; // index 
        std::string redirection; // return aaa/aaaa/html
        std::string alias; // alias key
        std::string cgiPath; // location of the interpreter 
        std::string cgiExt; // cgi extension
        std::string error_page; // same as the previous class
        int client_max_body_size;
        std::vector<int> error_codes; // // // // 
        int Cmds;
    public :
        /// GETTERS AND SETTERS ///
        void    setLocationIndex(size_t index)
        {
            this->location_index = index;
        }
        std::vector<int> getLocationErrorCodesObject(void)
        {
            return (this->error_codes);
        }
        std::vector<std::string> getLocationMethodsObject(void)
        {
            return (this->HttpMethods);
        }
        int getLocationindexObject(void)
        {
            return (this->location_index);
        }
        int getCmbsObject(void)
        {
            return (this->client_max_body_size);
        }
        std::string getLocationErrorPageObject(void)
        {
            return (this->error_page);
        }
        std::string getLocationRootObject(void)
        {
            return (this->root);
        }
        std::string getLocationIndexObject(void)
        {
            return (this->index);
        }
        std::string getLocationRedirectionObject(void)
        {
            return (this->redirection);
        }
        bool getLocationIsAutoIndexObject(void)
        {
            return (this->is_auto_index ? true : false);
        }
        std::string getLocationNameObject(void)
        {
            return (this->location_name);
        }

        
        /// GETTERS AND SETTERS ///
        
        server_location(){}
        server_location(server_location & s)
        {

            this->location_index = s.location_index;
            this->location_name = s.location_name;
            this->root = s.root;
            this->is_auto_index = s.is_auto_index;
            this->HttpMethods = s.HttpMethods;
            this->index = s.index;
            this->alias = s.alias;
            this->cgiPath = s.cgiPath;
            this->cgiExt = s.cgiExt;
            this->error_codes = s.error_codes;
            this->error_page = s.error_page;
            // exit(0);
        }
        server_location operator=(server_location & s)
        {
            this->location_index = s.location_index;
            this->location_name = s.location_name;
            this->root = s.root;
            this->is_auto_index = s.is_auto_index;
            this->HttpMethods = s.HttpMethods;
            this->index = s.index;
            this->alias = s.alias;
            this->cgiPath = s.cgiPath;
            this->cgiExt = s.cgiExt;
            this->error_codes = s.error_codes;
            this->error_page = s.error_page;
            
            return (*this);
        }
        void    getErrorPage(std::string *keys, size_t size)
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
            // exit(0);
        }
        void    getIndexPage(std::string *keys, size_t size)
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
        void    getAutoIndex(std::string *keys, size_t size)
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
        void    getRoot(std::string *keys, size_t size)
        {
            if(size <= 1 || size > 2)
            {
                std::cout << "Error Missing Root Page\n";
                exit(0);
            }
            this->root = keys[size - 1];
            delete [] keys;
        }
        void    getRedirection(std::string *keys, size_t size)
        {
            if(size <= 1 || size > 2)
            {
                std::cout << "Error Redirection Arguments !\n";
                exit(0);
            }
            this->redirection = keys[size - 1];
            delete [] keys;
        }
        void    getCmds(std::string *keys, size_t size)
        {
            if(size <= 1 || size > 2)
            {
                std::cout << "Error Cmbs Arguments";
                exit(0);
            }
            this->client_max_body_size = stoi(keys[size - 1]);
            delete [] keys;
        }
        void    getMethods(std::string *Keys, size_t size)
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
        void    getLocationName(std::string *Keys, size_t size)
        {
            if(size <= 1 || size > 3)
            {
                std::cout << "Error Location Name Arguments";
                exit(0);
            }
            this->location_name = Keys[size - 2];
            delete [] Keys;
        }
        void construct_location(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
        {
            std::vector<std::string> locationVec(first, last);
            size_t temp_size;
            this->client_max_body_size = 0;
            this->is_auto_index = false;
            for(size_t i = 0; i < locationVec.size(); i++)
            {
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
            // for(size_t i = 0; i < tokens.size(); i++)
            //     std::cout << " << " << tokens[i] << ">> \n";
            // exit(0);
            std::string *ret = new std::string[tokens.size()]; // ret string allocation;
            std::copy(tokens.begin(), tokens.end(), ret);
            for(size_t i = 0; i < tokens.size(); i++)
            {
                ret[i].erase(0, ret[i].find_first_not_of(" \t\r\n"));
                ret[i].erase(ret[i].find_last_not_of(" \t\r\n") + 1);
                // std::cout << ret[i] << std::endl;
            }
            
            // delete [] ret;
            *index_save = tokens.size();
            // exit(0);
            return (ret);
        }
        ~server_location(){}
        
};

class server_parser : public server_location
{
    protected :
        int port; // 8080
        std::string host; // 0.0.0.1
        int server_index; // server index in parsing
        size_t location_count; // number of location objects for looping purposes
        int client_max_body_size; // i mean that obvious
        std::vector<int> error_codes; // list of error codes linked with error path

        std::string root; // root key 
        std::string index; // index key
        std::string error_page; // error page index
        std::vector<std::string> server_names; // obvious
        std::string redirection; // return /eee/rrr

        server_location *location; // location objects
        bool is_auto_index; // is autoindex or not ? 
    public :
        server_parser() {};
        server_parser(server_parser & s)
        {
            this->port = s.getPortObject();
            this->host = s.getHostObject();
            this->server_index = s.getServer_IndexLocationObject();
            this->location_count = s.getLocationCount();
            this->client_max_body_size = s.getCmbsObject();
            this->redirection = s.getRedirectionObject();
            this->is_auto_index = s.getIsAutoIndexObject();
            this->root = s.getRootObject();
            this->server_names = s.getServerNamesObject();
            // if(this->location)
            //     delete [] this->location;
            this->location = new server_location[this->location_count];
            for(size_t i = 0;i < location_count; i++)
                this->location[i] = s.location[i];
        }
        server_parser & operator=(server_parser & s)
        {
            this->port = s.getPortObject();
            this->host = s.getHostObject();
            this->server_index = s.getServer_IndexLocationObject();
            this->location_count = s.getLocationCount();
            this->client_max_body_size = s.getCmbsObject();
            this->redirection = s.getRedirectionObject();
            this->is_auto_index = s.getIsAutoIndexObject();
            this->root = s.getRootObject();
            this->server_names = s.getServerNamesObject();
            // if(this->location)
            //     delete [] this->location;
            this->location = new server_location[this->location_count];
            for(size_t i = 0;i < location_count; i++)
                this->location[i] = s.location[i];
            return (*this);
        }
        /////// GETTERS AND SETTERS /////////////
        server_location *getServerLocationsObject(void)
        {
            return (this->location);
        }
        std::vector<std::string> getServerNamesObject(void)
        {
            return (this->server_names);
        }
        int getPortObject(void)
        {
            return (this->port);
        }
        std::string getHostObject(void)
        {
            return (this->host);
        }
        int getServer_IndexLocationObject(void)
        {
            return (this->location_index);
        }
        size_t getLocationCount(void)
        {
            return (this->location_count);
        }
        int getCmbsObject(void)
        {
            return (this->client_max_body_size);
        }
        std::vector<int> getErrorCodesObject(void)
        {
            return (this->error_codes);
        }
        std::string getIndexObject(void)
        {
            return (this->index);
        }
        std::string getRootObject(void)
        {
            return (this->root);
        }
        std::string getRedirectionObject(void)
        {
            return (this->redirection);
        }
        server_location *getServerLocation(void)
        {
            return (this->location);
        }
        bool getIsAutoIndexObject(void)
        {
            return (this->is_auto_index);
        }
        int getServerLocationCountObject(void)
        {
            return (this->location_count);
        }
        int getServerIndexObject(void)
        {
            return (this->server_index);
        }
        std::string getServerErrorPageObject(void)
        {
            return (this->error_page);
        }
        /////// GETTERS AND SETTERS /////////////
        bool is_digits(const std::string &str)
        {
            return str.find_first_not_of("0123456789") == std::string::npos;
        }
        void    getPort(std::string *Port, size_t temp_size)
        {
            std::string *temparray;
            size_t temp_sizee;
            if(temp_size > 2)
            {
                std::cout << "Error listen has more than one argument !\n";
                exit(0);
            }
            if(!strncmp(Port[1].c_str(), "localhost:", 10))
            {
                temparray = stringSplit(Port[1], ':', &temp_sizee);
                if (temp_sizee > 2)
                {
                    std::cout << "Error port has more than one location\n";
                    exit(0);
                }
                
                this->port = stoi(temparray[1]);
                this->host = temparray[0];
                std::cout << this->host << "<<<<< \n";
                // exit(0);
                delete [] temparray;
                // exit(0);
            }
            if (temp_size == 2 && is_digits(Port[1]))
            {
                this->port = stoi(Port[1]);
            }
            delete [] Port;
        }
        void    getServerName(std::string *keys, size_t size)
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
            // exit(0);
        }
        void    getErrorPage(std::string *keys, size_t size)
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
            // exit(0);
        }
        void    getIndexPage(std::string *keys, size_t size)
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
        void    getAutoIndex(std::string *keys, size_t size)
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
        void    getRoot(std::string *keys, size_t size)
        {
            if(size <= 1 || size > 2)
            {
                std::cout << "Error Missing Root Page\n";
                exit(0);
            }
            this->root = keys[size - 1];
            delete [] keys;
        }
        void    getRedirection(std::string *keys, size_t size)
        {
            if(size <= 1 || size > 2)
            {
                std::cout << "Error Redirection Arguments !\n";
                exit(0);
            }
            this->redirection = keys[size - 1];
            delete [] keys;
        }
        void    getCmds(std::string *keys, size_t size)
        {
            if(size <= 1 || size > 2)
            {
                std::cout << "Error Cmbs Arguments";
                exit(0);
            }
            this->client_max_body_size = stoi(keys[size - 1]);
            delete [] keys;
        }
        void    construct_server(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
        {
            this->port = 8080;
            this->is_auto_index = false;
            this->client_max_body_size = 0;
            std::vector<std::string> serverVec(first, last);
            size_t opening_bracket = 0;
            size_t closing_bracket = 0;
            size_t location_index = 0;
            size_t temp_size;
            this->location_count = getLocationCount(serverVec);
            this->location = new server_location[this->location_count]; // locations allocation
            setLocationsIndex(this->location);

            for(size_t i = 1; i < serverVec.size(); i++)
            {
                if(!strncmp(serverVec[i].c_str(), "listen ", 7))
                {
                    getPort(stringSplit(serverVec[i], ' ', &temp_size), temp_size); // host and port parsing;
                    if(this->host.size() == 0)
                        host = "localhost";

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
                    // exit(0);
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
                    this->location[location_index++].construct_location(serverVec.begin() + opening_bracket, serverVec.begin() + closing_bracket);                    
               }
                
            }

        }
        void    setLocationsIndex(server_location *location)
        {
            for(size_t i = 0; i < this->location_count;i++)
                location[i].setLocationIndex(i);
        }
        void    setServerIndex(int index){ this->server_index = index;}
        ~server_parser() {}
        size_t getLocationCount(std::vector<std::string> vec)
        {
            size_t count = 0;
            for(size_t i = 0; i < vec.size(); i++)
            {
                if (!strncmp(vec[i].c_str(), "location ", 9) && vec[i].back() == '{')
                    count++;
            }
            return (count);
        }

};

class config_parser : public server_parser
{
    private :
        size_t server_count;
        server_parser *servers;
    public :
        config_parser() {};
        config_parser(config_parser & c)
        {
            // if()
            this->server_count = c.getServerCountObject();
            servers = new config_parser[c.getServerCountObject()];
            for(size_t i = 0; i < this->server_count; i++)
                this->servers[i] = c.servers[i];
        }
        config_parser & operator=(config_parser & c)
        {
            // if(this->servers)
            //     delete [] this->servers;
            this->server_count = c.getServerCountObject();
            servers = new config_parser[c.getServerCountObject()];
            for(size_t i = 0; i < this->server_count; i++)
                this->servers[i] = c.servers[i];
            return (*this);
        }
        /////// GETTERS AND SETTERS //////////
        size_t getServerCountObject(void)
        {
            return (this->server_count);
        }
        server_parser *getServersObject(void)
        {
            return (this->servers);
        }
        config_parser(std::string filename)
        {
            std::vector<std::string> tempConf;
            std::ifstream file(filename);
            std::string line;
            int         server_index = 0;
            int         opening_bracket;
            int         closing_bracket;
            while(std::getline(file, line))
            {
                line.erase(0, line.find_first_not_of(" \t\r\n"));
                line.erase(line.find_last_not_of(" \t\r\n;") + 1);
                std::replace(std::begin(line),std::end(line),'\t',' ');
                std::replace(std::begin(line),std::end(line),'\n',' ');
                tempConf.push_back(line);
            }
            file.close();
            this->server_count = getServersCount(tempConf);
            this->servers = new server_parser[this->server_count]; // server allocation
            this->servers_index_init(); // indexing the serves depending in their position

            for(std::vector<std::string>::size_type i = 0; i < tempConf.size(); i++)
            {
                if (!strncmp(tempConf[i].c_str(), "server {", 8) && tempConf[i].back() == '{')
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
                    if (servers + server_index)
                    {
                        servers[server_index].construct_server(tempConf.begin() + opening_bracket, tempConf.begin() + closing_bracket + 1); // constructing server
                    }
                    server_index += 1;
                }
            }

        }
        void    servers_index_init()
        {
            for(size_t i = 0;i < this->server_count; i++)
                this->servers[i].setServerIndex(i);
        }
        int    getServersCount(std::vector<std::string> vec)
        {
            int count = 0;
            for(std::vector<std::string>::size_type i = 0; i < vec.size(); i++)
                {
                    if (!strncmp(vec[i].c_str(), "server ", 7) && vec[i].back() == '{')
                        count++;  
                }
                return (count);
            }
        
        ~config_parser(){ delete [] this->servers; }
        };

//////////////////////////////////
        std::ostream & operator<<(std::ostream & os, server_location & s)
        {
            std::vector<int> locationErrorCodes = s.getLocationErrorCodesObject();
            std::vector<std::string> locationMethods = s.getLocationMethodsObject();
            os << "     |------------->>\n";
            os << "     | Location index : " << s.getLocationindexObject() << '\n';
            os << "     | Location  Name : " << s.getLocationNameObject() << '\n';
            os << "     | Location CMBS : " << s.getCmbsObject() << '\n';
            for(size_t i = 0;i < locationErrorCodes.size();i++)
                os << "     | Location Error codes : " << locationErrorCodes[i] << '\n';
            os << "     | Location Error Page : " << s.getLocationErrorPageObject() <<  '\n';
            os << "     | Location Root : " << s.getLocationRootObject() << '\n';
            os << "     | Location Index Page : " << s.getLocationIndexObject() << '\n';
            os << "     | Location Redirection : " << s.getLocationRedirectionObject() << '\n';
            for(size_t i = 0; i < locationMethods.size(); i++)
                os << "     | Location Methods : "<< locationMethods[i] << '\n';
             os << "       | Location AutoIndex : " << s.getLocationIsAutoIndexObject() << '\n'; 
             return (os);
        }
        std::ostream & operator<<(std::ostream & os, server_parser & s)
        {
            std::vector<int> serverErrorCodes = s.getErrorCodesObject();
            std::vector<std::string> serverNames = s.getServerNamesObject();
            server_location *serverLocations = s.getServerLocationsObject();
            os << "|------------->>\n";
            os << "| Server index : " << s.getServerIndexObject() << '\n';
            os << "| Server Port : " << s.getPortObject() << '\n';
            os << "| Server Host : " << s.getHostObject() << '\n';
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
            for(size_t i = 0; i < s.getLocationCount(); i++)
                os << serverLocations[i] << '\n';
            return (os);
        }
        std::ostream & operator<<(std::ostream & os, config_parser & p)
        {
            server_parser *servers = p.getServersObject();
            for(size_t i = 0; i < p.getServerCountObject(); i++)
                os << servers[i] << '\n';
            return (os);
        }
#endif
