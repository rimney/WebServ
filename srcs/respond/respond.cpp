/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 20:32:17 by rimney            #+#    #+#             */
/*   Updated: 2023/03/16 16:31:11 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/respond.hpp"

std::string respond::gethttpVersion(void)
{
    return (this->httpVersion);
}

std::string respond::getstatusCode(void)
{
    return (this->statusCode);
}

std::string respond::getstatusDescription(void)
{
    return (this->statusDescription);
}

std::string respond::getContentLenght(void)
{
    return (this->ContentLenght);
}

std::string respond::getBody(void)
{
    return (this->Body);
}

std::string respond::getfinalString(void)
{
    return (this->finalString);
}

void    respond::sethttpVersion(std::string httpVersion)
{
    this->httpVersion = httpVersion;
}

void    respond::setstatusCode(std::string statusCode)
{
    this->statusCode = statusCode;
}

void    respond::setstatusDescription(std::string statusDescription)
{
    this->statusDescription = statusDescription;
}

void    respond::setContentLenght(std::string ContentLenght)
{
    this->ContentLenght = ContentLenght;
}

void    respond::setBody(std::string Body)
{
    this->Body = Body;
}

void    respond::setFinalString(std::string finalString)
{
    this->finalString = finalString;
}


void    respond::setRespond(server_parser server, std::string method, std::string path, std::string httpVersion, std::string error)
{
    if(!error.empty())
    {
        if(error == "501 Not Implemented")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("501");
            this->setstatusDescription("Not Implemented");
            this->setBody("<h1>\n501 Not Implemented\n</h1>");
            this->setContentLenght(std::to_string(this->getBody().size()));
            // set final string !!
        }
        else if(error == "400 Bad Request")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("400");
            this->setstatusDescription("Bad Request");
            this->setBody("<h1>\n400 Bad Request\n</h1>");
            this->setContentLenght(std::to_string(this->getBody().size()));
            // set final string !!
        }
        else if(error == "413 Request Entity Too Large")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("413");
            this->setstatusDescription("Request Entity Too Large");
            this->setBody("<h1>\n413 Request Entity Too Large\n</h1>");
            this->setContentLenght(std::to_string(this->getBody().size()));
            // set final string !!
        }
        else if(error == "414 Request-URI Too Long")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("414");
            this->setstatusDescription("Request-URI Too Long");
            this->setBody("<h1>\n414 Request-URI Too Long\n</h1>");
            this->setContentLenght(std::to_string(this->getBody().size()));
            // set final string !!
        }
        else if(error == "400 Bad Request")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("400");
            this->setstatusDescription("400 Bad Request");
            this->setBody("<h1>\n400 Bad Request\n</h1>");
            this->setContentLenght(std::to_string(this->getBody().size()));
            // set final string !!
        }
            
    }
    if(error.empty() && method == "GET")
    {
        std::file
        std::string fileHolder = fileExist(server.getRootObject(), path);
        
        exit(0);
    }
    
}