#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "/Users/brmohamm/Desktop/WebServ/includes/lexer.hpp"

struct Start_line
{
    std::string method;
    std::string path;
    std::string vertion;
};

class Request 
{

    private:
        Start_line start_line;
        std::map<std::string,std::string> header;
        std::vector<std::string> body;
    public:
        Request() {}
        Request(std::string value)
        {
            token _token;
            lexer _lexer(value);
            uintptr_t p = reinterpret_cast<uintptr_t>(&start_line);
            std::string *t = reinterpret_cast<std::string *>(p);
            for(int i = 0 ;_token._type != TYPE_EOF;)
            {
                _token = _lexer.get_next_token();
                if(_token._type == TYPE_END_OF_LINE)
                    i++;
                if(i == 0)
                {
                    *t = _token.value;
                    // std::cout << " ****: " <<_token.value << "   "  << _token._type<< std::endl;
                    t++;
                }
                _token.value.clear();
            }
        }
        ~Request(){}
        Start_line get_start_line(){return start_line;}
        std::map<std::string,std::string> get_header(){return header;}
        std::vector<std::string> get_body(){return body;}

};


#endif