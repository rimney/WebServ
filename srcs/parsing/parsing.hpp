/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:53:42 by rimney            #+#    #+#             */
/*   Updated: 2023/02/27 03:04:50 by rimney           ###   ########.fr       */
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
            std::cout << "location contructor called\n";
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
            std::cout << "location destructor called\n";
        }
        
};

class server_parser : public server_location
{
    private :
        int port;
        int host;
        std::string server_name;
        std::string error_page;
        int cmds;
        std::string root;
        std::string index;
        server_location *location;
    public :
        server_parser();
        server_parser(std::string filename)
        {
            
        }
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

// class parsingConfFile
// {
//     private :    
//         server *server;

//     public :
//         parsingConfFile() {};
//         parsingConfFile(std::string filename)
//         {
//             std::cout << "pa called\n";
//             std::string line;
//             std::string key;
//             std::vector<std::string> currentValues;
//             std::ifstream file(filename);
//             while(std::getline(file, line))
//             {
//                 std::size_t pos = line.find('#');
//                 if(pos != std::string::npos)
//                     line.erase(pos);
//                 line.erase(0, line.find_first_not_of(" \t\r\n"));
//                 line.erase(0, line.find_last_not_of(" \t\n\r") + 1);
//                 if(line.empty())
//                     continue ;
            
//             if(line.back() == '{')
//             {
//                 key = line.substr(0, line.size() - 1);
//                 currentValues.clear();
//             }
//             else if (line == "}")
//             {
//                 this->conf[key] = currentValues;
//                 std::cout << key << "confkey\n";
//             }
//             else
//             {
//                 currentValues.push_back(line);
//             }
//             }      
//             file.close(); 
//         }
//         std::map<std::string, std::vector<std::string> > getConf(void)
//         {
//             return (this->conf);
//         }
        
        
// };

#endif