/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:53:42 by rimney            #+#    #+#             */
/*   Updated: 2023/03/01 00:43:10 by rimney           ###   ########.fr       */
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
        std::string  *HttpMethods; // GET, POST, DELETE
        std::string index; // index 
        std::string rediection; // return aaa/aaaa/html
        std::string alias; // alias key
        std::string cgiPath; // location of the interpreter 
        std::string cgiExt; // cgi extension
        std::string error_page_location; // same as the previous class
        std::vector<int> error_codes; // // // // 
    public :
        server_location(){}
        void construct_location(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
        {
            std::vector<std::string> locationVec(first, last);
            for(size_t i = 1;i < locationVec.size(); i++)
            {
                std::cout << ">> " << locationVec[i] << '\n'; 
            }
            std::cout << "location index > " << this->location_index << '\n';
            
        }
        void        setLocationIndex(size_t index)
        {
            this->location_index = index;
        }
        void        setName(std::string name){this->location_name = name;}
        std::string getName(void){return (this->location_name);}
        void    setRoot(std::string root){this->root = root;}
        std::string getRoot(void){return (this->root);}
        bool    getAutoIndex(void){return (this->is_auto_index);}
        void    setAutoIndex(bool autoIndex){this->is_auto_index = autoIndex;}
        void        setAllowMethods(std::string *allowMethods){this->HttpMethods = allowMethods;}
        std::string        *getAllocMethods(void){return (this->HttpMethods);}
        void    setIndex(std::string index){this->index = index;}
        std::string getIndex(void){return (this->index);}
        std::string    getRedirection(void){return (this->rediection);}
        void    setRedirection(std::string redirection){this->rediection = redirection;}
        void    setAlias(std::string alias){this->alias = alias;}
        std::string getAlias(void){return (this->alias);}
        void    setCgiPath(std::string cgiPath){this->cgiPath = cgiPath;}
        std::string    getCgiPath(void){return (this->cgiPath);}
        void    setCgiExt(std::string cgiExt){this->cgiExt = cgiExt;}
        std::string getCgiExt(void){return (this->cgiExt);}
        std::string *stringSplit(std::string split)
        {
            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = 0;
            while ((end = split.find(' ', start)) != std::string::npos)
            {
                tokens.push_back(split.substr(start, end - start));
                start = end + 1;
            }
            tokens.push_back(split.substr(start));
            for(size_t i = 0; i < tokens.size(); i++)
                std::cout << " << " << tokens[i] << ">> \n";
            // exit(0);
            return (NULL);
        }
        ~server_location(){}
        
};

class server_parser : public server_location
{
    protected :
        int port; // 8080
        std::string host; // 0.0.0.1
        int server_index; // server index in parsing
        int location_count; // number of location objects for looping purposes
        int client_max_body_size; // i mean that obvious
        std::vector<int> error_codes; // list of error codes linked with error path

        std::string root; // root key 
        std::string index; // index key
        std::string error_page; // error page index
        std::string server_name; // obvious
        std::string redirection; // return /eee/rrr

        server_location *location; // location objects
        bool is_auto_index; // is autoindex or not ? 
    public :
        server_parser() {};
        void    construct_server(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
        {
            std::vector<std::string> serverVec(first, last);
            size_t opening_bracket = 0;
            size_t closing_bracket = 0;
            size_t location_index = 0;
            std::cout << "\n/////////////////// server " << this->server_index << "//////////////\n";
            this->location_count = getLocationCount(serverVec);
            this->location = new server_location[this->location_count]; // locations allocation
            setLocationsIndex(this->location);
            for(size_t i = 1; i < serverVec.size(); i++)
            {
                if(!strncmp(serverVec[i].c_str(), "listen ", 7))
                {
                    std::cout << serverVec[i] << '\n';
                    exit(0);
                }
                // std::cout << serverVec[i] << '\n';
                if (!strncmp(serverVec[i].c_str(), "location ", 9) && serverVec[i].back() == '{')
                {
                    opening_bracket = i;
                    // std::cout << "\nLOCATION >>>" << serverVec[opening_bracket] << "\n";
                    i++;
                    while(serverVec[i] != "}")
                    {
                        // std::cout << serverVec[i] <<  "<< "<< '\n';
                        i++;
                    }
                    closing_bracket = i;
                    // std::cout << "\nclosing bracket >>> " << serverVec[closing_bracket] << '\n';
                    this->location[location_index++].construct_location(serverVec.begin() + opening_bracket, serverVec.begin() + closing_bracket);                    
                }
                // stringSplit(serverVec[i]);
            }
            std::cout << "/////////////////// server " << this->server_index << "//////////////\n\n"; 
        }
        void    setLocationsIndex(server_location *location)
        {
            for(size_t i = 0; i < this->location_count;i++)
                this->location[i].setLocationIndex(i);
        }
        void    setServerIndex(int index){ this->server_index = index;}
        ~server_parser() {};
        int getPort(void){ return (this->port);}
        std::string getHost(void){ return (this->host);}
        std::string getServerName(void){ return (this->server_name);}
        std::string getErrorPage(void){ return (this->error_page);}
        int getCmds(void){ return (this->client_max_body_size);}
        std::string getRoot(void){return (this->root);}
        std::string getIndex(void){return (this->index);}
        server_location *getLocations(void){return (this->location);}
        size_t getLocationCount(std::vector<std::string> vec)
        {
            size_t count = 0;
            for(size_t i = 0; i < vec.size(); i++)
            {
                if (!strncmp(vec[i].c_str(), "location ", 9) && vec[i].back() == '{')
                    count++;
            }
            std::cout << count << " < location count\n";
            return (count);
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
            bool        is_open = false;
            int         server_index = 0;
            int         opening_bracket;
            int         closing_bracket;
            while(std::getline(file, line))
            {
                line.erase(0, line.find_first_not_of(" \t\r\n"));
                line.erase(line.find_last_not_of(" \t\r\n") + 1);
                tempConf.push_back(line);
            }
            file.close();
            this->server_count = getServersCount(tempConf);
            std::cout << "Number Of Servers : " << this->server_count << '\n';
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
                    servers[server_index].construct_server(tempConf.begin() + opening_bracket, tempConf.begin() + closing_bracket + 1); // constructing server
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
        ~config_parser(){delete [] this->servers;}
        };

#endif
