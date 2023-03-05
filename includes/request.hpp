#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include "lexer.hpp"

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
        std::vector<std::string> body;
        std::map<std::string,std::string> header;
    public:
        Request() {}
        Request(std::string value)
        {
            parser(value);
        }
        void parser(std::string value)
        {
            token _token;
            lexer _lexer(value);
            std::string buffer;

            uintptr_t p = reinterpret_cast<uintptr_t>(&start_line);
            std::string *t = reinterpret_cast<std::string *>(p);
            for(int i = 0 ;_token.type != TYPE_EOF;)
            {
                _token = _lexer.get_next_token();
                if(_token.type == TYPE_END_OF_LINE)
                    i++;
                if(i == 0)
                {
                    *t = _token.value;
                    t++;
                }
                else if (_token.type != TYPE_END_OF_LINE 
                        && _token.type != TYPE_END_OF_SSECTION && _token.type != TYPE_TWO_POINT)
                {
                    _lexer.get_next_token();
                    header.insert(std::make_pair(_token.value,_lexer.get_next_token().value));
                }
                else if(_token.type == TYPE_END_OF_SSECTION)
                {
                    while(_token.type != TYPE_EOF)
                    {
                        while(_token.type != TYPE_END_OF_SSECTION && _token.type != TYPE_EOF)
                        {
                            buffer += _token.value;
                            _token = _lexer.get_next_token();
                        }
                        _token = _lexer.get_next_token();
                        body.push_back(buffer);
                        buffer.clear();
                    }
                }
                _token.value.clear();
            }
            // std::map<std::string, std::string>::iterator itr;
            // std::cout << "\nThe heder is : \n";
            // std::cout << "\tKEY\tELEMENT\n";
            // for (itr = header.begin(); itr != header.end(); ++itr) {
            //     std::cout << '\t' << itr->first << '\t' << itr->second << '\n';
            // }
            // std::cout << "**" << std::endl;
            // std::cout <<  body.front() << std::endl;

            // std::vector<std::string>::iterator itrv;
            // std::cout << "\nThe body is : \n";
            // for (itrv = body.begin(); itrv != body.end(); ++itrv) {
            //     std::cout << *itrv << '\n';
            // }
        }
        ~Request(){}
        Start_line &get_start_line(){return start_line;}
        std::map<std::string,std::string> &get_header(){return header;}
        std::vector<std::string> &get_body(){return body;}

};


#endif