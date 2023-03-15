/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney <rimney@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:53:42 by rimney            #+#    #+#             */
/*   Updated: 2023/03/15 01:47:07 by rimney           ###   ########.fr       */
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
        int							Cmds;
        int							location_index;
        int							client_max_body_size; // buffer of what im supposed so send in response
        bool						is_auto_index; // is autoindex
        std::string					location_name; // /location/example
        std::string					root; // root key
        std::string					index; // index 
        std::string					redirection; // return aaa/aaaa/html
        std::string					cgiPath; // location of the interpreter 
        std::string					cgiExt; // cgi extension
        std::string					error_page; // same as the previous class
        std::vector<int>			error_codes; // // // // 
        std::vector<std::string>	HttpMethods; // GET, POST, DELETE
    public :
		////////////// Constructors and Overloads //////////////
        server_location(){};
        ~server_location(){};
        server_location(server_location & s);
        server_location operator=(server_location const & s);
		////////////// Constructors and Overloads //////////////
		
		////////////// Getters and Setters //////////////
        int							getCmbsObject(void);
        int							getLocationindexObject(void){return (this->location_index);}
        int							getLocationCmbsObject(void) const ;
        bool						getLocationIsAutoIndexObject(void) const{return (this->is_auto_index ? true : false);}
        std::string					getLocationIndexObject(void) const {return (this->index);}
        std::string					getLocationRedirectionObject(void) const{return (this->redirection);}
        std::string					getLocationErrorPageObject(void) const {return (this->error_page);}
        std::string					getLocationRootObject(void) const {return (this->root);}
        std::string					getLocationNameObject(void) const {return this->location_name;}
        std::vector<int>			getLocationErrorCodesObject(void) {return (this->error_codes);}
        std::vector<std::string>	getLocationMethodsObject(void){return (this->HttpMethods);}
		////////////// Getters and Setters //////////////

		////////////// Parsing Fucntions //////////////		
        void    setLocationIndex(size_t index) {this->location_index = index;}
        void    getIndexPage(std::string *keys, size_t size);
        void    getErrorPage(std::string *keys, size_t size);
        void    getAutoIndex(std::string *keys, size_t size);
        void    getRoot(std::string *keys, size_t size);
        void    getRedirection(std::string *keys, size_t size);
        void    getCmds(std::string *keys, size_t size);
        void    getMethods(std::string *Keys, size_t size);
        void    getLocationName(std::string *Keys, size_t size);
        void    construct_location(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last);
		////////////// Parsing Fucntions //////////////
};

class server_parser : public server_location
{
    protected :
        int							port; // 8080
        int							host; // 0.0.0.1 --> into in int
        int							server_index; // server index in parsing
        int							client_max_body_size; // i mean that obvious
        size_t						location_count; // number of location objects for looping purposes
        std::string					root; // root key 
        std::string					index; // index key
        std::string					error_page; // error page index
        std::string 				redirection; // return /eee/rrr
        std::vector<int>			error_codes; // list of error codes linked with error path
        std::vector<std::string>	server_names; // obvious

        server_location *location; // location objects
        bool is_auto_index; // is autoindex or not ? 
    public :
		////////////// Constructors and Overloads //////////////
        server_parser(){};
        ~server_parser() {} ;
        server_parser(server_parser & s);
        server_parser & operator=(server_parser const  & s);
		////////////// Constructors and Overloads //////////////

		////////////// Getters and Setters //////////////
        int							getPortObject(void)	const;
        int							getServer_IndexLocationObject(void) const;
        int							getHostObject(void) const;
        int							getCmbsObject(void) const;
        int							getServerLocationCountObject(void) const;
        int							getServerIndexObject(void) const;
        bool						getIsAutoIndexObject(void) const;
        std::string					getIndexObject(void) const;
        std::string					getRootObject(void) const;
        std::string					getRedirectionObject(void) const;
        std::string					getServerErrorPageObject(void) const;
        server_location				*getServerLocationsObject(void) const;
        std::vector<int>			getErrorCodesObject(void) const;
        std::vector<std::string>	getServerNamesObject(void) const;
		////////////// Getters and Setters //////////////

		////////////// Parsing Fucntions //////////////		
        int				ipToInt(std::string host);
        void			setServerIndex(int index);
        void			getCmds(std::string *keys, size_t size);
        void			getRoot(std::string *keys, size_t size);
        void    		getErrorPage(std::string *keys, size_t size);
        void	  		getPort(std::string *Port, size_t temp_size);
        void			getIndexPage(std::string *keys, size_t size);
        void			getAutoIndex(std::string *keys, size_t size);
        void			getServerName(std::string *keys, size_t size);
        void			getRedirection(std::string *keys, size_t size);
        void			setLocationsIndex(server_location *location);
        void			construct_server(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last);
        bool			is_digits(const std::string &str);
        size_t			getLocationCount(void) const;
        size_t 			getLocationCount(std::vector<std::string> vec);
        server_location	*getServerLocation(void) const;
		void			getServerDataFromRootLocation(void);
        void            restoreRootObject(int i);
        void            restoreIndexObject(int i);

		////////////// Parsing Fucntions //////////////		


};

class config_parser : public server_parser
{
    private :
        size_t			server_count;
        server_parser	*servers;
    public :
	////////////// Constructors //////////////
        config_parser(){} ;
        config_parser(config_parser & c);
        config_parser(std::string filename);
        ~config_parser() {} ;
	////////////// Constructors //////////////
        config_parser & operator=(config_parser const & c);
        size_t			getServerCountObject(void) const; // server count getter
        server_parser	*getServersObject(void);
        void			getServerName(std::string *keys, size_t size);
        void			servers_index_init();
        int				getServersCount(std::vector<std::string> vec);
};

std::ostream &	operator<<(std::ostream& os, config_parser& p);
std::ostream &	operator<<(std::ostream & os, server_parser & s);
std::ostream &	operator<<(std::ostream & os, server_location & s);
std::string		fileExist(server_parser server, std::string root, std::string file);
char			*toIp(int ip);
std::string *stringSplit(std::string split, char c, size_t *index_save);

#endif
