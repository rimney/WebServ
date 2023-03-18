/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:42:24 by rimney            #+#    #+#             */
/*   Updated: 2023/03/18 18:19:28 by rimney           ###   ########.fr       */
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
        server_parser server;
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
        
};

bool theFileExists(const std::string& fileName);
bool isDirectory(const char* path);
std::string	isFileOrDirectory(std::string path);


#endif
