/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney <rimney@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 20:32:17 by rimney            #+#    #+#             */
/*   Updated: 2023/04/03 00:33:48 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"

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

size_t respond::getContentLenght_sizet(void)
{
    return (this->Body.size());
}

bool respond::getBodyFlag(void)
{
    return (this->bodyFlag);
}

void respond::setBodyFlag(bool v)
{
    this->bodyFlag = v;
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
    this->ContentLenght = std::to_string(Body.size());
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

std::string & respond::getPathSave(void)
{
    return (this->pathSave);
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


std::string respond::chunkedFileToString(std::string path)
{
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) 
    {
        std::cerr << "Error opening file " << path << std::endl;
        return "";
    }
    
    // Move file pointer to current chunk position
    lseek(fd, this->chunkPosition, SEEK_SET);
    
    char buffer[CHUNK_SIZE];
    int bytes_read = read(fd, buffer, CHUNK_SIZE);
    if (bytes_read == 0)
    {
        std::cout << "THE END !\n";
        bodyFlag = false;
        pathSave.clear();
        this->cleanAll();
        this->chunkPosition = 0;
        return ("0\r\n\r\n");
    }
    else if (bytes_read == -1)
    {
        std::cerr << "Error while chunking";
        return "";
    }
    
    std::string content(buffer, bytes_read);
    
    // Update current chunk position
    this->chunkPosition += bytes_read;
    
    close(fd);
    
    if (getfinalString().size() > 0) {
        return getfinalString() + content;
    }
    else
        finalString.clear();
    return content;
}

std::string respond::getFileType(const std::string& fileName)
{
    // Get the file extension
    std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
    // Check the file extension and return the content-type
    if (ext == "txt")
        return "text/plain";
    else if (ext == "jpg" || ext == "jpeg")
        return "image/jpeg";
    else if (ext == "png")
        return "image/png";
    else if (ext == "pdf")
        return "application/pdf";
    else if (ext == "html")
        return "text/html";
    else if (ext == "css")
        return "text/css";
    else if(ext == "mp4")
        return "video/mp4";
    else if (ext == "mp3")
        return ("audio/mp3");
    else
        return "text/html";
}


void    respond::cleanAll(void)
{
    this->Body.clear();
    this->httpVersion.clear();
    this->statusCode.clear();
    this->statusDescription.clear();
    this->ContentLenght.clear();
    this->finalString.clear();
    this->content_type.clear();
    this->location.clear();
}

std::string	respond::getAutoIndexPage(std::string path)
{
    std::string temp;
    DIR *dir;
    struct dirent *ent;
    
    temp = "<html>\n<head>\n<title>Directory listing</title>\n</head>\n<body>\n<ul>\n";
    if ((dir = opendir(path.c_str())) != NULL)
    { // replace "." with the path to the directory you want to list
        while ((ent = readdir (dir)) != NULL)
        {
            if (ent->d_type == DT_REG || ent->d_type == DT_DIR) { // list regular files and directories
            std::string type = (ent->d_type == DT_REG) ? "file" : "directory";
            temp = temp + "<li><a href=\"" + ent->d_name + "\">" + ent->d_name + "</a> (" + type + ")</li>\n";
        }
    }
    closedir (dir);
    }
    else
    {
        std::cout << "Unable to open directory";
        return "";
    }
    temp = temp + "</ul>\n";
    temp = temp + "</body>\n";
    temp = temp + "</html>\n";
    
    return (temp);
}

std::string respond::getLocation(void)
{
    return ("Location: " + this->location);
}

void	respond::setExpires(std::string expires)
{
	this->expireDate = expires;
}

std::string respond::getExpires(void)
{
	return ("Expires: " + this->expireDate);
}

std::string respond::getCookies(void)
{
	return ("Set-cookie: " + this->cookies);
}

void	respond::setCookies(std::string Cookie)
{
	this->cookies = Cookie;
}


void    respond::setLocation(std::string location)
{
    this->location = location;
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


std::string		respond::setErrorBody(std::string status_code)
{
    if(theFileExists(this->server.getServerErrorPageObject()) && this->isAmongErrorCodes(atoi(status_code.c_str())))
        return (fileToSring(this->server.getServerErrorPageObject()));
    else if(this->isAmongErrorCodes(atoi(status_code.c_str())))
        return (fileToSring(this->server.getServerErrorPageObject()));    
    return ("");   
}

std::string		respond::mergeRespondStrings(void)
{
    if (this->location.empty() && this->cookies.empty() && this->expireDate.empty())
	    this->finalString =  this->gethttpVersion() + " " + this->getstatusCode() + " " + this->getstatusDescription() + "\r\nContent-Length: " + this->getContentLenght() + "\r\n" + this->content_type + "\r\n\r\n" + this->getBody();
    else if(this->cookies.empty() && this->location.size() > 0)
	    this->finalString = this->gethttpVersion() + " " + this->getstatusCode() + " " + this->getstatusDescription() + "\r\nContent-Length: " + this->getContentLenght() + "\r\n" + this->getLocation() + "\r\n" +  this->content_type + "\r\n\r\n" + this->getBody();
	else 
	    this->finalString = this->gethttpVersion() + " " + this->getstatusCode() + " " + this->getstatusDescription() + "\r\nContent-Length: " + this->getContentLenght() + "\r\n" + this->getCookies() + "\r\n" + getExpires() + "\r\n" + this->getLocation() + "\r\n" +  this->content_type + "\r\n\r\n" + this->getBody();
	
	return (this->finalString);
}

void    respond::recoverBody(int status_code)
{
    if(status_code == 404)
    {
        this->setFinalString("HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 22\r\n\r\n<h1>404 Not Found</h1>\r\n");
        this->setContentLenght(std::to_string(this->getfinalString().size()));
    }
    else if(status_code == 501)
    {
        this->setFinalString("HTTP/1.1 501 Not Implementedr\nContent-Type: text/html\r\nContent-Length: 29\r\n\r\n<h1>501 Not Implemented</h1>\r\n");
        this->setContentLenght(std::to_string(this->getfinalString().size()));
    }
    else if(status_code == 400)
    {
        this->setFinalString("HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 22\r\n\r\n<h1>\n400 Bad Request\n</h1>");
        this->setContentLenght(std::to_string(this->getfinalString().size()));
    }
    else if(status_code == 413)
    {
        this->setFinalString("HTTP/1.1 413 Request Entity Too Large\r\nContent-Type: text/html\r\nContent-Length: 41\r\n\r\n<h1>\n413 Request Entity Too Large\n</h1>\r\n");
        this->setContentLenght(std::to_string(this->getfinalString().size()));
    }
    else if(status_code == 414)
    {
        this->setFinalString("HTTP/1.1 414 Request-URI Too Longer\nContent-Type: text/html\r\nContent-Length: 37\r\n\r\n<h1>\n414 Request-URI Too Long\n</h1>\r\n");
        this->setContentLenght(std::to_string(this->getfinalString().size()));
    }
    else if (status_code == 403)
    {
        this->setFinalString("HTTP/1.1 403 Error Forbidden\r\nContent-Type: text/html\r\nContent-Length: 31\r\n\r\n<h1>\n 403 Error Forbidden</h1>\r\n");
        this->setContentLenght(std::to_string(this->getfinalString().size()));
        
    }
    else if (status_code == 405)
    {
        this->setFinalString("HTTP/1.1 405 Error Forbidden\r\nContent-Type: text/html\r\nContent-Length: 31\r\n\r\n<h1>\n 405 Error Forbidden</h1>\r\n");
        this->setContentLenght(std::to_string(this->getfinalString().size()));
        
    }
}

void    respond::setContentType(std::string const & content_type)
{
    this->content_type = "Content-Type: " + content_type;
}

void	respond::setRespond(std::string path, std::string httpVersion, std::string error)
{
    if(!error.empty())
    {
        if(error == "501 Not Implemented")
        {
            this->bodyFlag = false;
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
            this->bodyFlag = false;
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
            this->bodyFlag = false;
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
            this->bodyFlag = false;
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
            this->bodyFlag = false;
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
            this->bodyFlag = false;
            this->sethttpVersion(httpVersion);
            this->setstatusCode("400");
            this->setstatusDescription("400 Bad Request");
            this->setBody(this->setErrorBody(this->getstatusCode()));
            this->setContentLenght(std::to_string(this->getBody().size()));
			this->mergeRespondStrings();
            close(fd);
            return ;
        }
        else if(error == "404")
        {
            this->bodyFlag = false;
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
            this->bodyFlag = false;
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
            std::cout << path << " << path\n";
			server_location location = server.getOneLocationObject(this->location_index);
	        int index;

			this->bodyFlag = false;
			this->sethttpVersion(httpVersion);
			this->setstatusCode("301");
			this->setstatusDescription("Moved Permanently");
			this->setContentLenght(std::to_string(this->getBody().size()));
	        
	        if(location.getLocationRedirectionObject().substr(0, 5) == "http:" || location.getLocationRedirectionObject().substr(0, 6) == "https:")
	        {
	            this->setLocation(location.getLocationRedirectionObject());
	           	this->mergeRespondStrings();
	        }
	        else
	        {
				index = this->server.getLocationByName(location.getLocationRedirectionObject());
				if(index != -1 || path.size() > 0)
				{
					if(path[path.size() - 1] == '/')
						path = path.substr(0, path.length() - 1);
                    {
                        if(location.getLocationRedirectionObject().size())
					    {
                            this->setLocation(location.getLocationRedirectionObject() + '/');
					        this->mergeRespondStrings();
					        return ;
                        }
                        else
                        {
                            this->setLocation(path + '/');
					        this->mergeRespondStrings();
					        return ;
                        }
                    }
				}
				else
				{
                    std::cout << index << " << index\n";
                    std::cout << path << " <<<< EEEEEEE\n";
                    exit(0);
					this->setRespond(path, this->httpVersion, "404");
					return ;
				}
			std::cout << this->finalString << " <<\n";
			return ;
			}
		}
		else if(error == "201") // << HERE
		{
			this->bodyFlag = false;
            this->sethttpVersion(httpVersion);
            this->setstatusCode("201");
            this->setstatusDescription("Created");
            setContentType("text/plain");
            this->setBody("Crated");
            this->setContentLenght(std::to_string(this->getBody().size()));
			this->mergeRespondStrings();
            return ;
        } 
    }
    if(theFileExists(path) == false || isFileOrDirectory(path) == "error")
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
        this->pathSave = path;
        cleanAll();
        if(this->getBodyFlag() == false)
        {
            std::cout << "Header Set !\n"; 
            this->setChunkPosition(0);
            this->setContentType(getFileType(path));
            this->sethttpVersion(httpVersion);
            this->setstatusCode("200");
            this->setstatusDescription("OK");
            this->setContentLenght(std::to_string(fileToSring(path).size()));
            this->Body.clear();
            if(getfinalString().size() == 0)
		        this->mergeRespondStrings();
        }
        return ;
    }
}

int respond::getChunkPosition(void)
{
    return (this->chunkPosition);
}

void respond::setChunkPosition(int chunk)
{
    this->chunkPosition = chunk;
}

bool isDirectory(const char* path) {
    struct stat file_stat;
    if (stat(path, &file_stat) != 0) {
        return false;
    }
    return S_ISDIR(file_stat.st_mode);
}

std::string	isFileOrDirectory(std::string path) // need to fix this one !!
{
    std::ifstream init(path);
    if (isDirectory(path.c_str()))
	{
        return ("directory");
    }
	else if(init.good())
        return ("file");
	return ("error");
}

respond respond::getRespond(void)
{
    return (*this);    
}