/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 20:32:17 by rimney            #+#    #+#             */
/*   Updated: 2023/03/20 22:21:42 by eel-ghan         ###   ########.fr       */
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

bool theFileExists(const std::string& fileName)
{
    std::ifstream infile(fileName);
    if(infile.good())
        return (true);
    return (false);
}

bool respond::isAmongErrorCodes(int error_code)
{
    std::vector<int> errors = server.getErrorCodesObject();
    for(size_t i = 0;i < errors.size(); i++)
    {
        if(error_code == errors[i])
            return (true);
    }
    return (false);
}

std::string     respond::fileToSring(std::string path)
{
    std::ifstream file(path);
    if(!file.good())
        return (setErrorBody("404"));
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    return (content);
}

std::string     respond::errorStringToString(int error)
{
    if(error == 501)
        return ("<h1>\n501 Not Implemented\n</h1>\n");
    else if(error == 400)
        return ("<h1>\n400 Bad Request\n</h1>\n");
    else if(error == 413)
        return ("h1>\n413 Request Entity Too Large\n</h1>\n");
    else if(error == 414)
        return ("<h1>\n414 Request-URI Too Long\n</h1>\n");
    else if(error == 400)
        return ("<h1>\n400 Bad Request\n</h1>\n");
    else if(error == 404)
        return ("<h1>\n404 Page Not Found\n</h1>\n");
    else if(error == 403)
        return ("<h1>\n 403 Error Forbidden</h1>\n");
    return ("OK");
}

std::string		respond::setErrorBody(std::string status_code)
{
    if(theFileExists(this->server.getServerErrorPageObject()) && this->isAmongErrorCodes(atoi(status_code.c_str())))
        return (fileToSring(this->server.getServerErrorPageObject()));
    else if(this->isAmongErrorCodes(atoi(status_code.c_str())))
        return (fileToSring(this->server.getServerErrorPageObject()));    
    return (errorStringToString(atoi(status_code.c_str())));
}

std::string		respond::mergeRespondStrings(void)
{
	std::string response = this->gethttpVersion() + " " + this->getstatusCode() + " " + this->getstatusDescription() + "\r\nContent-Length: " + this->getContentLenght() + "\r\n\r\n" + this->getBody();
	this->finalString = response;
	return (response); 
}

void	respond::setRespond(std::string path, std::string httpVersion, std::string error)
{
    if(!error.empty())
    {
        if(error == "501 Not Implemented")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("501");
            this->setstatusDescription("Not Implemented");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
            this->mergeRespondStrings();
            return ;
        }
        else if(error == "403")
        {
            std::cout << "respond\n";
            this->sethttpVersion(httpVersion);
            this->setstatusCode("403");
            this->setstatusDescription("Forbidden");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
            this->mergeRespondStrings();
            return ;
        }
        else if(error == "400 Bad Request")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("400");
            this->setstatusDescription("Bad Request");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
            this->mergeRespondStrings();
            return ;
        }
        else if(error == "413 Request Entity Too Large")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("413");
            this->setstatusDescription("Request Entity Too Large");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
            this->mergeRespondStrings();
            return ;
        }
        else if(error == "414 Request-URI Too Long")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("414");
            this->setstatusDescription("Request-URI Too Long");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
            this->mergeRespondStrings();
            return ;
        }
        else if(error == "400 Bad Request")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("400");
            this->setstatusDescription("400 Bad Request");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
			this->mergeRespondStrings();
            return ;
        }
        else if(error == "404")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("404");
            this->setstatusDescription("Not Found");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
			this->mergeRespondStrings();
            return ;
        }
        else if(error == "405")
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("405");
            this->setstatusDescription("Method Not Allowed");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
			this->mergeRespondStrings();
            return ;
        
        }
        else if(error == "301") // << HERE
        {
            this->sethttpVersion(httpVersion);
            this->setstatusCode("301");
            this->setstatusDescription("Moved Permanently");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
			this->mergeRespondStrings();
            return ;
        }   
    }
    if(theFileExists(path) == false && isFileOrDirectory(path) == "file")
    {
        this->sethttpVersion(httpVersion);
        this->setstatusCode("404");
        this->setstatusDescription("Not Found");
        this->setBody(this->setErrorBody(this->getstatusCode()));
        this->setContentLenght(std::to_string(this->getBody().size()));
		this->mergeRespondStrings();
        return ;
    }
    else
    {
        this->sethttpVersion(httpVersion);
        this->setstatusCode("200");
        this->setstatusDescription("OK");
        this->setBody(this->setErrorBody(this->getstatusCode()));   
        this->setContentLenght(std::to_string(this->getBody().size()));
		this->mergeRespondStrings();
        return ;
    }
}

bool isDirectory(const char* path) {
    struct stat file_stat;
    if (stat(path, &file_stat) != 0) {
        return false;
    }
    return S_ISDIR(file_stat.st_mode);
}

std::string	isFileOrDirectory(std::string path)
{
    if (isDirectory(path.c_str()))
	{
        return ("directory");
    }
	else {
        return ("file");
    }
	return ("error");
}