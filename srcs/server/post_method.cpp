#include "../../includes/server.hpp"

size_t remove_header(std::string &request, size_t i,std::string &buffer,std::string &filename)
{
    int count_qouet = 0;
    bool boundry = false;
     
    for(size_t j = i; j < (size_t)request.length();j++)
    {
        
        if(request[j] != '-' &&  request[j] < '0' && request[j] > '9')
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
        if(i >= request.length() )
        {
            std::ofstream post(filename);
            buffer.erase(request.length() - 3 , request.length() - 1);
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
                    if(!filename.empty())
                    {
                        std::ofstream post(filename);
                        post << buffer;
                        post.close();
                        filename.clear();
                        buffer.clear();
                        count_qouet = 0;
                    }
                    for(;request[i] != '"' ;i++)
                        filename += request[i];
                }
                if(request[i] == '"')
                    count_qouet++;
            }
        }
    }
    return i ;
}

void    server::post_method()
{

    std::string filename;
    std::string buffer;
    
    if(!request.get_body().empty())
    {
        // std::cout << "\nThe body is : \n"; 
        // std::cout << "*" << request.get_body() << "*"<< std::endl;
        //  std::cout << "*" << request.get_body().length() << "*"<< std::endl;
        for (size_t i = 0 ; i < (size_t)request.get_body().length();i++)
        {
            if(request.get_body()[i] == '-')
                i = remove_header(request.get_body(),i,buffer,filename);
            if(i < (size_t)request.get_body().length())
            {
                buffer += request.get_body()[i];
                std::cout << request.get_body()[i];
            }

        }
    }

}

