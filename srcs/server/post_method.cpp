#include "../../includes/server.hpp"
#include "../../includes/request.hpp"

size_t remove_header(std::string &request, size_t i,std::string &buffer,std::string &filename,std::string &upload)
{
    int count_qouet = 0;
    bool boundry = false;
     
    for(size_t j = i; j < (size_t)request.length();j++)
    {
        std::cerr << request[j] << std::endl;
        if(request[j] != '-' &&  request[j] < 48 && request[j] > 57)
            break;
        else if(request[j + 1] == 13 && request[j + 2] == 10)
        {
            i = j + 3;
            boundry = true;
            break;
        }
    }
    if(boundry == true)
    {
        if(i >= request.length() || !filename.empty())
        {
            std::ofstream post(upload + filename);
            buffer.erase(buffer.length() - 2 , 2);
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
    }
    return i ;
}

void    server::post_method(server_parser &serv)
{
    std::string extention;
    std::string boundary;
    std::string filename;
    std::string buffer;
    (void)serv;
    std::string upload = "/Users/brmohamm/Desktop/WebServ/upload/";
    std::cout << request.get_body() << std::endl;
    if(!request.get_body().empty())
    {
        //!serv.getServerLocationsObject()[request.get_start_line().location_index].getLocationNameObject().empty()
        if(!upload.empty())
        {
            if(!request.get_header().find("Content-Type")->first.empty())
            {
                if(strncmp(request.get_header().find("Content-Type")->second.c_str(),"multipart",9) == 0)//multipart
                {
                    for(size_t i = request.get_header().find("Content-Type")->second.find("=") + 1; i < (size_t)request.get_header().find("Content-Type")->second.length();i++)
                    {
                        if (request.get_header().find("Content-Type")->second[i] != '-')
                               boundary +=  request.get_header().find("Content-Type")->second[i];
                    }
                    // std::cout << ">> " <<request.get_body()[request.get_body().find(boundary,request.get_body().find(boundary) + 1) - 32]  << std::endl;
                    // for (size_t i = 0 ; i < (size_t)request.get_body().length();i++)
                    // {
                    //     if(request.get_body()[i] == '-')
                    //         i = remove_header(request.get_body(),i,buffer,filename,upload);
                    //     if(i < (size_t)request.get_body().length())
                    //         buffer += request.get_body()[i];
                    // }
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

