#include "../../includes/server.hpp"
#include "../../includes/request.hpp"

size_t remove_header(std::string &request, size_t i,std::string &buffer,std::string &filename,std::string &upload)
{
    int count_qouet = 0;
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
        for(; i < (size_t)request.length();i++)
        {
            if(request[i] == 13 && request[i + 1] == 10 
                && request[i + 2] == 13 && request[i + 3] == 10)
            {
                    i += 4;
                    break;
            }
            if(count_qouet == 3)
            {   
                for(;i < (size_t)request.length() && request[i] != '"' ;i++)
                    filename += request[i];
                count_qouet = 0;
            }
            else if(request[i] == '"')
                count_qouet++;
        }
    }
    return i ;
}

void    server::post_method(server_parser &serv)
{
    std::string extention;
    std::string boundary;
    long boundary_pos = 0;
    std::string filename;
    std::string buffer;
    bool is_boundary = false;
    (void)serv;
    std::string upload = "/Users/brmohamm/Desktop/WebServ/upload/";
    // std::cout << request.get_body() << std::endl;
    if(!request.get_body().empty())
    {
        //!serv.getServerLocationsObject()[request.get_start_line().location_index].getLocationNameObject().empty()
        if(!upload.empty())
        {
            if(!request.get_header().find("Content-Type")->first.empty())
            {
                if(strncmp(request.get_header().find("Content-Type")->second.c_str(),"multipart",9) == 0)//multipart
                {
                    for(size_t i = request.get_header().find("Content-Type")->second.find("=") + 1; i < (size_t)request.get_header().find("Content-Type")->second.length();i++)//find boundary
                    {
                        if (request.get_header().find("Content-Type")->second[i] != '-')
                               boundary +=  request.get_header().find("Content-Type")->second[i];
                    }
                    for (size_t i = 0 ; i < (size_t)request.get_body().length();i++)
                    {
                        boundary_pos = request.get_body().find(boundary,request.get_body().find(boundary) + boundary_pos);
                        i = remove_header(request.get_body(),i,buffer,filename,upload);
                        for(; i < (size_t)request.get_body().length();i++)
                        {
                            if((request.get_body()[i] == 13 && request.get_body()[i + 1] == 10 && request.get_body()[i + 2] == '-') 
                                || (request.get_body()[i ] == 10 && request.get_body()[i + 1] == '-') 
                                || (request.get_body()[i ] == 13 && request.get_body()[i + 1] == '-'))
                            {
                                if(request.get_body().find(boundary,request.get_body().find(boundary) + i) < i + 32)
                                {
                                    break;
                                    is_boundary = true;
                                }
                                is_boundary = false;
                            }  
                            if(is_boundary == false)
                                buffer += request.get_body()[i];
                        }
                    }
                }
                else
                {
                    buffer = request.get_header().find("Content-Type")->second;
                    for(int i = 0 ; i < (int)buffer.length();i++)
                        if(buffer[i] == '/')
                            for(int j = i + 1 ; j < (int)buffer.length();j++)
                                extention += buffer[j];
                        std::ofstream post(upload + request.get_header().find("Postman-Token")->second +'.'+ extention);
                        post << request.get_body();
                        post.close();
                }
            } 
        }
        else
        {
            //cgi
        }

    }
   
}

