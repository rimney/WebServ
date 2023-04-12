/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:42:24 by rimney            #+#    #+#             */
/*   Updated: 2023/04/12 22:49:44 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef RESPOND_HPP
# define RESPOND_HPP
# include "../includes/parsing.hpp"
# include "sys/stat.h"
# include <cstdio>
# include <dirent.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <string>

class respond
{
    private:
        bool			bodyFlag;
        int             chunkPosition;
        std::string		Body; // content
        std::string		statusCode; // 200 404 501
        std::string		httpVersion; // http/1.1
	    std::string		ContentLenght; // len of body
        std::string		finalString; //
        std::string		content_type;
        std::string		statusDescription;
		std::string		pathSave;
        std::string     location;
        server_parser	server;
		std::string		tempAutoIndexFile;// set a header flag << // set a path to the file
        int				location_index;
        int             fd;
        std::vector<std::string>     cookies;
        std::string     expireDate;
    
    public:
        respond() {};
        respond(server_parser const & s): bodyFlag(false), server(s) {};
        respond(respond const & r) {*this = r;}
        void        setRespondServer(server_parser s) {this->server = s;}
		void		setRespondLocationIndex(int location_index) {this->location_index = location_index;}
		int			getRespondLocationIndex(void) {return (this->location_index);}
        std::string gethttpVersion(void);
        std::string getstatusCode(void);
        std::string getstatusDescription(void);
        std::string getContentLenght(void);
        std::string getBody(void);
        std::string getfinalString(void);
        int         getFd(void);
        void        setFd(int fd) {this->fd = fd;}
        void        sethttpVersion(std::string httpVersion);
        void        setstatusCode(std::string statusCode);
        void        setstatusDescription(std::string statusDescription);
        void        setContentLenght(std::string ContentLenght);
        void        setBody(std::string Body);
        void        setLocation(std::string  location);
        void        setCookies(std::string Cookie);
        std::string getCookies(void);
        void        setExpires(std::string expires);
        std::string getExpires(void);
        std::string getLocation(void);
        void        setFinalString(std::string finalString);
        void        setRespond(std::string path, std::string httpVersion, std::string error);
        std::string setErrorBody(std::string status_code);
        bool        isAmongErrorCodes(int error_code);
        std::string fileToSring(std::string path);
        std::string	errorStringToString(int error);
        std::string	mergeRespondStrings(void);
        void        recoverBody(int status_code);
        size_t      getContentLenght_sizet(void);
        void		cleanAll(void);
        std::string	getFileType(const std::string& fileName);
        void		setContentType(std::string const & content_type);
        std::string	getAutoIndexPage(std::string path);
        bool        getBodyFlag();
        void        setBodyFlag(bool v);
        std::string & getPathSave(void);
        void    setBodyChunked(std::string s);
        std::string chunkedFileToString(std::string path);
        respond     getRespond(void);
        int         getChunkPosition(void);
        void        setChunkPosition(int chunk);

        

        
};

bool theFileExists(const std::string& fileName);
bool isDirectory(const char* path);
std::string	isFileOrDirectory(std::string path);


#endif
