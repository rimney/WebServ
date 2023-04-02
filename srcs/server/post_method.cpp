#include "../../includes/server.hpp"
#include "../../includes/request.hpp"


int is_file_or_dir_(std::string & path)
{
    struct stat file_info;

    if (stat(path.c_str(), &file_info) == -1)
        return -1;
    if (file_info.st_mode & S_IFDIR)
        return 2;
    if (file_info.st_mode & S_IFREG)
        return 1;
    return -1;
}

size_t remove_header(std::string & request, size_t i,std::string &buffer,std::string &filename,std::string upload)
{
    long pos = 0;
    if(i >= request.length() || !filename.empty())
    {
        std::ofstream post(upload + filename);
        post << buffer;
        post.close();
        filename.clear();
        buffer.clear();
    }
    else
    {
        
        pos =request.find("filename=" , i);
        std::cout <<"****==== pos " << pos << std::endl;
        if(pos != -1)
        {
            for(size_t t = pos + 10; request[t] != '"' ;t++)
                filename += request[t];
            pos = request.find("\r\n\r\n",pos);
            if(pos != -1)
                return pos + 4;
        }
        else
        {
            pos = request.find("\r\n\r\n",i);
            if(pos != -1)
                return pos + 4;
        }
    }
    return i ;
}
void server::multi_part(server_parser &serv,int fd)
{
    bool is_boundary = false;
    std::string boundary;
    std::string filename;
    std::string buffer;
    for(size_t i = _request[fd].get_header().find("Content-Type")->second.find("=") + 1; i < (size_t)_request[fd].get_header().find("Content-Type")->second.length();i++)//find boundary
    {
        if (_request[fd].get_header().find("Content-Type")->second[i] != '-')
                boundary +=  _request[fd].get_header().find("Content-Type")->second[i];
    }
    for (size_t i = 0 ; i < (size_t)_request[fd].get_body().length();i++)
    {
        i = remove_header(_request[fd].get_body(),i,buffer,filename,serv.getServerLocationsObject()[_request[fd].get_start_line().location_index].getUploadObject());
        for(; i < (size_t)_request[fd].get_body().length();i++)
        {
            if((_request[fd].get_body()[i] == 13 && _request[fd].get_body()[i + 1] == 10 && _request[fd].get_body()[i + 2] == '-') 
                || (_request[fd].get_body()[i ] == 10 && _request[fd].get_body()[i + 1] == '-') 
                || (_request[fd].get_body()[i ] == 13 && _request[fd].get_body()[i + 1] == '-'))
            {
                if(_request[fd].get_body().find(boundary,_request[fd].get_body().find(boundary) + i) < i + 32)
                {
                    break;
                    is_boundary = true;
                }
                is_boundary = false;
            }  
            if(is_boundary == false)
                buffer += _request[fd].get_body()[i];
        }
    }
}

void    server::post_method(server_parser &serv, int fd)
{
    std::string extention;
    std::string boundary;
    std::string buffer;
    int is_dir_or_not = 0;
    std::string error = "";
    DIR* dir;
    struct dirent* entry;
    //  std::cout <<"**";
    // for(size_t i = 0; i < _request[fd].get_body().length(); i++)
    // {
    //     if(_request[fd].get_body()[i] == 13)
    //         std::cout <<"</r>";
    //     else if (_request[fd].get_body()[i] == 10)
    //         std::cout <<"</n>";
    //     if (_request[fd].get_body()[i] != 13)
    //         std::cout <<_request[fd].get_body()[i];
        
    // }
    // std::cout <<" *end* " << "\n";

    
    if(!_request[fd].get_body().empty())
    {
        if(!serv.getServerLocationsObject()[_request[fd].get_start_line().location_index].getUploadObject().empty())
        {
            if(!_request[fd].get_header().find("Content-Type")->first.empty())
            {

                if(strncmp(_request[fd].get_header().find("Content-Type")->second.c_str(),"multipart",9) == 0)//multipart
                    multi_part(serv,fd);
                else
                {
                    struct timeval tp;
                    gettimeofday(&tp, NULL);
                    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
                    buffer = _request[fd].get_header().find("Content-Type")->second;
                    for(int i = 0 ; i < (int)buffer.length();i++)
                        if(buffer[i] == '/')
                            for(int j = i + 1 ; j < (int)buffer.length();j++)
                                extention += buffer[j];
                    std::ofstream post(serv.getServerLocationsObject()[_request[fd].get_start_line().location_index].getUploadObject() + std::to_string(ms) +'.'+ extention);//check /
                    post << _request[fd].get_body();
                    post.close();
                }
            } 
            error = "201";
        }
        else
        {
           is_dir_or_not =  is_file_or_dir_(_request[fd].get_start_line().full_path);
           if(is_dir_or_not == -1 )//notfound
           {
                error = "404";
           }
           else if(is_dir_or_not == 2 ) //dir
           {
                bool is_found = false;
                std::cout << " ** "<< _request[fd].get_start_line().location_index << "++"<< std::endl;
                if(_request[fd].get_start_line().full_path[_request[fd].get_start_line().full_path.length() - 1] != '/')
                {
                    error = "301";
                    std::cout << ">>>>>>>>>>>>>>>> 301 <<<<<<<<<<" << std::endl;
                    //add "/" to uri and return it
                }
                else
                {
                    if ((dir = opendir(_request[fd].get_start_line().full_path.c_str())) != 0) 
                    {
                        while ((entry = readdir(dir)) != 0) 
                        {
                            if (entry->d_type == DT_REG)
                            {
                                if(strncmp(entry->d_name,"index.php",9) == 0)
                                    is_found = true;
                            }  // if the entry is a regular file
                        }
                        closedir(dir);
                    }
                    
                }
                if(!is_found && error == "")
                {
                        error = "403";//dosent have index file : "403 Forbidden"
                        std::cout << " ** "<< "errror 403" << "++"<< std::endl;
                }
                else if(!serv.getServerLocationsObject()[_request[fd].get_start_line().location_index].getCgiPathObject(_request[fd].get_start_line().full_path + "" ).empty() && is_found && error == "")
                {
                    std::cout << " ** "<< "<<<<<<<<< dir >>>>>>>>>" << "++"<< std::endl;
                    _request[fd].get_start_line().full_path += "index.php";
                    cgi_handler cgi(_server_config, _request[fd], _fd_port_map[fd]);
                    cgi.exec(_respond[fd]);
                    return;
                }
                else if(error == "")
                   error = "403";//location doesn’t have cgi : "403 Forbidden" 
                 
           }
           else//file
           {
                if(!serv.getServerLocationsObject()[_request[fd].get_start_line().location_index].getCgiPathObject(_request[fd].get_start_line().full_path).empty())
                {
                    std::cout <<"<<<<< query"  <<_request[fd].get_start_line().query << ">>>>>"<< std::endl;
                    std::cout << " ** "<< "<<<<<<<<<file >>>>>>>>>" << "++"<< std::endl;
                    cgi_handler cgi(_server_config, _request[fd], _fd_port_map[fd]);
                    cgi.exec(_respond[fd]);
                    return;
                }
                else
                   error = "403";//location doesn’t have cgi : "403 Forbidden" 
           }
        }
    }
    std::cout << ">>>>>>>>>>>>>>>>" << error << "<<<<<<<<<<" << std::endl;
    _respond[fd].setRespond(_request[fd].get_start_line().full_path, _request[fd].get_start_line().vertion,error);
    std::cout << _respond[fd].getfinalString();
    // exit(0);
   
}