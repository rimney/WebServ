/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 20:32:17 by rimney            #+#    #+#             */
/*   Updated: 2023/03/16 00:32:52 by rimney           ###   ########.fr       */
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

std::string checkFileExist(std::string root, std::string file)
{
    std::string fileHolder;
    std::ifstream init(file);
    if(init.good())
        return (file);
    else
    {
        // std::string s = "/";
        fileHolder = root + file;
        std::ifstream file(fileHolder);
        if (file.good())
            return (fileHolder);
        else
        {
            return ("not found");
        }
    }
    return ("not found");
}

void    respond::setRespond(server_parser server, std::string method, std::string path, std::string httpVersion)
{
    if(path == "/")
        path = "index.html";
    
    std::string fileHolder = checkFileExist(server.getRootObject(), path);
    std::cout << fileHolder;
    this->sethttpVersion(httpVersion);
    if(method == "GET")
    {
        std::cout << path << ">> path\n";
        exit(0);
    }
}