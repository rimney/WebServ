/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:53:42 by rimney            #+#    #+#             */
/*   Updated: 2023/04/07 06:08:24 by eel-ghan         ###   ########.fr       */
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
        bool                        has_cgi;
        bool                        has_redirection;
        bool                        has_301_code;
        std::string					location_name; // /location/example
        std::string					root; // root key
        std::string					index; // index 
        std::string					redirection; // return aaa/aaaa/html
        std::string					error_page; // same as the previous class
        std::vector<int>			error_codes; // // // //
        std::vector<std::string>	cgiExt; // cgi extension
        std::string                 upload;
        std::vector<std::string>	cgiPaths; // location of the interpreter 
        std::vector<std::string>	HttpMethods; // GET, POST, DELETE
        
    public :
		////////////// Constructors and Overloads //////////////
        server_location(){};
        ~server_location(){};
        server_location(server_location const & s);
        server_location & operator=(server_location const & s);
		////////////// Constructors and Overloads //////////////
		
		////////////// Getters and Setters //////////////
        int							getLocationCmbsObject(void);
        int							getLocationindexObject(void){return (this->location_index);}
        int							getLocationCmbsObject(void) const ;
        bool						getLocationIsAutoIndexObject(void) const {return (this->is_auto_index);}
        std::string					getLocationIndexObject(void) const {return (this->index);}
        std::string					getLocationRedirectionObject(void) const{return (this->redirection);}
        std::string					getLocationErrorPageObject(void) const {return (this->error_page);}
        std::string					getLocationRootObject(void) const {return (this->root);}
        std::string					getLocationNameObject(void) const {return this->location_name;}
        std::vector<int>			getLocationErrorCodesObject(void) {return (this->error_codes);}
        std::vector<std::string>	getLocationMethodsObject(void){return (this->HttpMethods);}
        bool                        getLocationHas301Code(void) {return (this->has_301_code);}

        std::string                 getCgiPathObject(std::string path);
        std::string                 getUploadObject(void) {return (this->upload);}
        bool                        getHasCgi(void) {return (has_cgi);}
        bool                        getHasRedirection(void) {return (this->has_redirection);}
        
		////////////// Getters and Setters //////////////

		////////////// Parsing Fucntions //////////////		
        void    setLocationIndex(size_t index) {this->location_index = index;}
        void    getIndexPage(std::string *keys, size_t size);
        void    getErrorPage(std::string *keys, size_t size);
        void    getAutoIndex(std::string *keys, size_t size);
        void    getRoot(std::string *keys, size_t size);
        void    getRedirection(std::string *keys, size_t size);
        void    getLocationCmds(std::string *keys, size_t size);
        void    getMethods(std::string *Keys, size_t size);
        void    getCgiPath(std::string *Keys, size_t size);
        void    getCgiExec(std::string *Keys, size_t size);
        void    getUpload(std::string *Keys, size_t size);
        void    getLocationName(std::string *Keys, size_t size);
        void    checkCgiAllowed(void);
        void    construct_location(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last);
		bool    isCgi(std::string path);
        ////////////// Parsing Fucntions //////////////
};

class server_parser : public server_location
{
    protected :
        std::vector<int>							port; // 8080
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
        bool                        server_has_get_method;
        bool                        server_has_post_method;
        bool                        server_had_delete_method;

        std::vector<server_location> location; // location objects
        bool is_auto_index; // is autoindex or not ? 
    public :
		////////////// Constructors and Overloads //////////////
        server_parser(){};
        server_parser(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last){
            this->construct_server(first, last);
        }
        
        ~server_parser() {} ;
        server_parser(server_parser const & s);
        server_parser & operator=(server_parser const  & s);
		////////////// Constructors and Overloads //////////////

		////////////// Getters and Setters //////////////
        std::vector<int>							getPortObject(void)	const;
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
        std::vector<server_location> getServerLocationsObject(void) const;
        std::vector<int>			getErrorCodesObject(void) const;
        std::vector<std::string>	getServerNamesObject(void) const;
        server_location             getOneLocationObject(int index) const;
		////////////// Getters and Setters //////////////

		////////////// Parsing Fucntions //////////////		
        int				ipToInt(std::string host);
        void			setServerIndex(int index);
        void			getCmds(std::string *keys, size_t size);
        void			getRoot(std::string *keys, size_t size);
        void    		getErrorPage(std::string *keys, size_t size);
        void            getHost(std::string *keys, size_t size);
        void	  		getPort(std::string *Port, size_t temp_size);
        void			getIndexPage(std::string *keys, size_t size);
        void			getAutoIndex(std::string *keys, size_t size);
        void			getServerName(std::string *keys, size_t size);
        void			getRedirection(std::string *keys, size_t size);
        void			setLocationsIndex(std::vector<server_location> location);
        void			construct_server(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last);
        bool			is_digits(const std::string &str);
        size_t			getLocationCount(void) const;
        size_t 			getLocationCount(std::vector<std::string> vec);
        std::vector<server_location>	getServerLocation(void) const;
		void			getServerDataFromRootLocation(void);
        void            restoreRootObject(int i);
        void            restoreIndexObject(int i);
        void            restoreAutoIndex(int i);
        void            restoreCmbs(int i);
        void restoreServerMethods(int i);
        int getLocationByName(std::string name) const;
        void    checkServerData(void);



		////////////// Parsing Fucntions //////////////		


};

class config_parser : public server_parser
{
    private :
        size_t			server_count;
        std::vector<server_parser>	servers;
    public :
	////////////// Constructors //////////////
        config_parser(){} ;
        config_parser(config_parser & c);
        config_parser(std::string filename);
        ~config_parser() {} ;
	////////////// Constructors //////////////
        config_parser & operator=(config_parser const & c);
        size_t			getServerCountObject(void) const; // server count getter
        std::vector<server_parser>	getServersObject(void) const;
        void			getServerName(std::string *keys, size_t size);
        void			servers_index_init();
        int				getServersCount(std::vector<std::string> vec);
        bool            hasDuplicatePorts();
        void            checkFileBrackets(std::string file);
};

std::ostream &	operator<<(std::ostream& os, config_parser& p);
std::ostream &	operator<<(std::ostream & os, server_parser & s);
std::ostream &	operator<<(std::ostream & os, server_location & s);
std::string		fileExist(std::string root, std::string file);
char			*toIp(int ip);
std::string *stringSplit(std::string split, char c, size_t *index_save);

#endif
