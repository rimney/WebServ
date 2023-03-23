/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:42:24 by rimney            #+#    #+#             */
/*   Updated: 2023/03/22 19:55:42 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../includes/parsing.hpp"
# include "sys/stat.h"

class respond
{
    private:
        std::string httpVersion; // http/1.1
        std::string statusCode; // 200 404 501
        std::string statusDescription; // OK ERROR
	    std::string ContentLenght; // len of body
        std::string Body; // content
        std::string finalString; //
        std::string content_type;
        server_parser server; // set a header flag << // set a path to the file
    public:
        respond() {};
        respond(server_parser const & s): server(s) {};
        respond(respond const & r) {*this = r;}
        // respond & operator=(respond const & r)
        // {
        //     *this = r;
        //     return (*this);
        // }
        void        setRespondServer(server_parser s) {this->server = s;}
        std::string gethttpVersion(void);
        std::string getstatusCode(void);
        std::string getstatusDescription(void);
        std::string getContentLenght(void);
        std::string getBody(void);
        std::string getfinalString(void);
        void        sethttpVersion(std::string httpVersion);
        void        setstatusCode(std::string statusCode);
        void        setstatusDescription(std::string statusDescription);
        void        setContentLenght(std::string ContentLenght);
        void        setBody(std::string Body);
        void        setFinalString(std::string finalString);
        void        setRespond(std::string path, std::string httpVersion, std::string error);
        std::string setErrorBody(std::string status_code);
        bool        isAmongErrorCodes(int error_code);
        std::string fileToSring(std::string path);
        std::string errorStringToString(int error);
        std::string mergeRespondStrings(void);
        void        recoverBody(int status_code);
        size_t      getContentLenght_sizet(void);
        void        cleanAll(void);
        std::string getFileType(const std::string& fileName);
        void    setContentType(std::string const & content_type);
        // get_rest_body(path of file)

        
};

bool theFileExists(const std::string& fileName);
bool isDirectory(const char* path);
std::string	isFileOrDirectory(std::string path);


#endif
