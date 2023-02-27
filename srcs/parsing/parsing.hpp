/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:53:42 by rimney            #+#    #+#             */
/*   Updated: 2023/02/27 04:49:16 by rimney           ###   ########.fr       */
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

class serverParsing
{
    private :
        std::string key; // the line that includes the '{' bracket and must implemet a way how to find the other  one
        std::vector<std::string> *elems; // elements after finding the '{' and untill the '}' bracket
    public : 
        
};

class server_location
{
    protected :
        int     location_index;
        std::string name;
        std::string root;
        bool autoIndex;
        std::string  *allowMethods;
        std::string index;
        std::string rediection; // return aaa/aaaa/html
        std::string alias;
        bool is_cgi; // whether the given location is cgi interpreter or not
        std::string cgiPath; // location of the interpreter 
        std::string cgiExt; // cgi extension
    public :
        server_location()
        {
        }
        void        setName(std::string name)
        {
            this->name = name;
        }
        std::string getName(void)
        {
            return (this->name);
        }
        void    setRoot(std::string root)
        {
            this->root = root;
        }
        std::string getRoot(void)
        {
            return (this->root);
        }
        bool    getAutoIndex(void)
        {
            return (this->autoIndex);
        }
        void    setAutoIndex(bool autoIndex)
        {
            this->autoIndex = autoIndex;
        }
        void        setAllowMethods(std::string *allowMethods)
        {
            this->allowMethods = allowMethods;
        }
        std::string        *getAllocMethods(void)
        {
            return (this->allowMethods);
        }
        void    setIndex(std::string index)
        {
            this->index = index;
        }
        std::string getIndex(void)
        {
            return (this->index);
        }
        std::string    getRedirection(void)
        {
            return (this->rediection);
        }
        void    setRedirection(std::string redirection)
        {
            this->rediection = redirection;
        }
        void    setAlias(std::string alias)
        {
            this->alias = alias;
        }
        std::string getAlias(void)
        {
            return (this->alias);
        }
        void    setIsCgi(bool is_cgi)
        {
            this->is_cgi = is_cgi;
        }
        bool getIsCgi(void)
        {
            return (this->is_cgi);
        }
        void    setCgiPath(std::string cgiPath)
        {
            this->cgiPath = cgiPath;
        }
        std::string    getCgiPath(void)
        {
            return (this->cgiPath);
        }
        void    setCgiExt(std::string cgiExt)
        {
            this->cgiExt = cgiExt;
        }
        std::string getCgiExt(void)
        {
            return (this->cgiExt);
        }
        ~server_location()
        {
        }
        
};

class server_parser : public server_location
{
    protected :
        int server_index;
        int port;
        int host;
        std::string server_name;
        std::string error_page;
        int cmds;
        std::string root;
        std::string index;
        server_location *location;
    public :
        server_parser() {};
        server_parser(std::string filename)
        {
            std::ifstream file(filename);
            std::string line;
            while(std::getline(file, line))
            {
                std::cout << line << '\n';
            }
            file.close();
        }
        void    setServerIndex(int index)
        {
            this->server_index = index;
        }
        ~server_parser() {};
        int getPort(void)
        {
            return (this->port);
        }
        int getHost(void)
        {
            return (this->host);
        }
        std::string getServerName(void)
        {
            return (this->server_name);
        }
        std::string getErrorPage(void)
        {
            return (this->error_page);
        }
        int getCmds(void)
        {
            return (this->cmds);
        }
        std::string getRoot(void)
        {
            return (this->root);
        }
        std::string getIndex(void)
        {
            return (this->index);
        }
        server_location *getLocations(void)
        {
            return (this->location);
        }
        
};

class config_parser : public server_parser
{
    private :
        int server_count;
        server_parser *servers;
    public :
        config_parser() {};
        config_parser(std::string filename)
        {
            std::vector<std::string> tempConf;
            std::ifstream file(filename);
            std::string line;
            while(std::getline(file, line))
            {
                tempConf.push_back(line);
            }
            file.close();
            this->server_count = getServersCount(tempConf);
            std::cout << this->server_count << '\n';
            this->servers = new server_parser[this->server_count];
            this->servers_index_init();
            for(std::vector<std::string>::size_type i = 0; i < tempConf.size(); i++)
            {
                tempConf[i].erase(0, tempConf[i].find_first_not_of(" \t\r\n"));
                tempConf[i].erase(tempConf[i].find_last_not_of(" \t\r\n") + 1);
                std::cout <<  "<" << tempConf[i] << ">" << '\n';
            }
            exit(0);
        }
        void    servers_index_init()
        {
            for(size_t i = 0;i < this->server_count; i++)
                this->servers->setServerIndex(i);
        }
        int    getServersCount(std::vector<std::string> vec)
        {
            int count = 0;
            for(std::vector<std::string>::size_type i = 0; i < vec.size(); i++)
                {
                    if (!strncmp(vec[i].c_str(), "server", 6))
                        count++;  
                }
                return (count);
            }
        ~config_parser()
        {
            delete [] this->servers;
        }
        };

#endif
