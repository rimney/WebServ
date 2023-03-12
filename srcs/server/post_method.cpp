#include "../../includes/server.hpp"

void    server::post_method()
{
    std::string extention;
    std::string buffer;
    buffer = request.get_header().find("Content-Type")->second;
    for(int i = 0 ; i < (int)buffer.length();i++)
        if(buffer[i] == '/')
            for(int j = i + 1 ; j < (int)buffer.length();j++)
                extention += buffer[j];
    std::ofstream post("filename1." + extention);
    post << request.get_body();
    post.close();
}