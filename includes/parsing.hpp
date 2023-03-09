/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:53:42 by rimney            #+#    #+#             */
/*   Updated: 2023/03/09 02:17:13 by rimney           ###   ########.fr       */
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
        void    setLocationIndex(size_t index) {this->location_index = index;}
        std::vector<int> getLocationErrorCodesObject(void) {return (this->error_codes);}
        std::vector<std::string> getLocationMethodsObject(void){return (this->HttpMethods);}
        int getLocationindexObject(void){return (this->location_index);}
        int getCmbsObject(void);
        std::string getLocationErrorPageObject(void) const {return (this->error_page);}
        std::string getLocationRootObject(void) const {return (this->root);}
        std::string getLocationIndexObject(void) const {return (this->index);}
        std::string getLocationRedirectionObject(void) const{return (this->redirection);}
        bool getLocationIsAutoIndexObject(void) const{return (this->is_auto_index ? true : false);}
        std::string getLocationNameObject(void) const {return this->location_name;}
        server_location(){};
        server_location(server_location & s);
        server_location operator=(server_location const & s);
        int     getLocationCmbsObject(void) const ;
        void    getIndexPage(std::string *keys, size_t size);
        void    getErrorPage(std::string *keys, size_t size);
        void    getAutoIndex(std::string *keys, size_t size);
        void    getRoot(std::string *keys, size_t size);
        void    getRedirection(std::string *keys, size_t size);
        void    getCmds(std::string *keys, size_t size);
        void    getMethods(std::string *Keys, size_t size);
        void    getLocationName(std::string *Keys, size_t size);
        void construct_location(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last);
        std::string *stringSplit(std::string split, char c, size_t *index_save);
        ~server_location(){ };
};

class server_parser : public server_location
{
    protected :
        int port; // 8080
        int host; // 0.0.0.1 --> into in int
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
        server_parser(){};
        server_parser(server_parser & s);
        server_parser & operator=(server_parser const  & s);
        /////// GETTERS AND SETTERS /////////////;
        server_location *getServerLocationsObject(void) const;
        std::vector<std::string> getServerNamesObject(void) const;
        int getPortObject(void) const;
        int getHostObject(void) const;
        int getServer_IndexLocationObject(void) const;
        size_t getLocationCount(void) const;
        int getCmbsObject(void) const;
        std::vector<int> getErrorCodesObject(void) const;
        std::string getIndexObject(void) const;
        std::string getRootObject(void) const;
        std::string getRedirectionObject(void) const;
        server_location *getServerLocation(void) const;
        bool getIsAutoIndexObject(void) const;
        int getServerLocationCountObject(void) const;
        int getServerIndexObject(void) const;
        std::string getServerErrorPageObject(void) const;
        bool is_digits(const std::string &str);
        void    getPort(std::string *Port, size_t temp_size);
        void    getErrorPage(std::string *keys, size_t size);
        void    getIndexPage(std::string *keys, size_t size);
        void    getAutoIndex(std::string *keys, size_t size);
        void    getServerName(std::string *keys, size_t size);
        void    getRoot(std::string *keys, size_t size);
        void    getRedirection(std::string *keys, size_t size);
        void    getCmds(std::string *keys, size_t size);
        void    construct_server(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last);
        void    setLocationsIndex(server_location *location);
        void    setServerIndex(int index);
        int     ipToInt(std::string host);
        ~server_parser() {} ;
        size_t getLocationCount(std::vector<std::string> vec);

};

class config_parser : public server_parser
{
    private :
        size_t server_count;
        server_parser *servers;
    public :
        config_parser(){} ;
        config_parser(config_parser & c);
        config_parser & operator=(config_parser const & c);
        size_t getServerCountObject(void) const; // server count getter
        server_parser *getServersObject(void);
        config_parser(std::string filename);
        void    getServerName(std::string *keys, size_t size);
        void    servers_index_init();
        int    getServersCount(std::vector<std::string> vec);
        ~config_parser() {} ;
};

std::ostream& operator<<(std::ostream& os, config_parser& p);
std::ostream & operator<<(std::ostream & os, server_parser & s);
std::ostream & operator<<(std::ostream & os, server_location & s);

#endif
