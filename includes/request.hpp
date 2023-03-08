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
        std::string body1;
        std::map<std::string,std::string> header;
        std::string r_error;
    public:
        Request() {}
        Request(std::string value,config_parser &s)
        {
            parser(value);
            errors(s);
        }
        ~Request(){}
        void parser(std::string value)
        {
            token _token;
            lexer _lexer(value);
            std::string buffer;
            bool end_of_section = false;

            uintptr_t p = reinterpret_cast<uintptr_t>(&start_line);
            std::string *t = reinterpret_cast<std::string *>(p);
            for(int i = 0 ;_token.type != TYPE_EOF;)
            {
                _token = _lexer.get_next_token();
                if(_token.type == TYPE_END_OF_LINE || _token.type == TYPE_CR)
                    i++;
                 if(_token.type == TYPE_END_OF_SSECTION)
                    end_of_section = true;
                if(i == 0 && !end_of_section)
                {
                    *t = _token.value;
                    t++;
                }
                else if (_token.type != TYPE_END_OF_LINE && _token.type != TYPE_CR
                        && _token.type != TYPE_END_OF_SSECTION && _token.type != TYPE_TWO_POINT && _token.type == false)
                {
                    _lexer.get_next_token();
                    buffer = _lexer.get_next_token().value;
                    buffer.erase(buffer.begin());
                    if(!buffer.empty() && !_token.value.empty())
                    header.insert(std::make_pair(_token.value,buffer));
                    // std::cout << _token.value << buffer   << std::endl;
                    // std::cout << '\n' << "end" << '\n' << std::endl;
                    buffer.clear();
                }
                else if(_token.type == TYPE_END_OF_SSECTION)
                {
                    buffer.clear();
                    for (int i = _lexer.get_index(); i < (int)value.length(); i++)
                        buffer += value[i];
                    body1 = buffer;
                    break;
                }
                _token.value.clear();
            }
            // std::map<std::string, std::string>::iterator itr;
            // std::cout << "\nThe heder is : \n";
            // std::cout << "\tKEY\tELEMENT\n";
            // for (itr = header.begin(); itr != header.end(); ++itr) {
            //     std::cout << '\t'  << '*' << itr->first << '*' << '\t' << '*' << itr->second << '*' << '\n';
            // }
            // std::cout << "**" << std::endl;
            // std::cout <<  body.front() << std::endl;

            // std::vector<std::string>::iterator itrv;
            // std::cout << "\nThe body is : \n";
            // for (itrv = body.begin(); itrv != body.end(); ++itrv) {
            //     std::cout << *itrv << '\n';
            // }
        }
        void errors(config_parser &s)
        {
            server_parser *serv = s.getServersObject();
            std::string tmp_path = start_line.path;
            (void)s;
            if(!header.find("Transfer-Encoding")->first.empty())
            {
                if(header.find("Transfer-Encoding")->second != "chunked" )
                    r_error = "501 Not Implemented";
            }
            if((start_line.method == "POST" && header.find("Transfer-Encoding")->first.empty() && header.find("Content-Length")->first.empty())
                || (start_line.method == "POST" && !header.find("Transfer-Encoding")->first.empty() && !header.find("Content-Length")->first.empty()))
                r_error = "400 Bad Request";
            if(!header.find("Content-Length")->first.empty())
            {
                if(serv[0].getCmbsObject()  <  stoi(header.find("Content-Length")->second))
                r_error = "413 Request Entity Too Large";
            }
            if(tmp_path.length() > 2048)
                r_error = "414 Request-URI Too Long";
            for(int i = 0 ; i < (int)tmp_path.length() ; i++)
            {
                if(((tmp_path[i] < 95 || tmp_path[i] > 'z') && (tmp_path[i] < 33 || tmp_path[i] > 90))
                    || tmp_path[i] == 34 || tmp_path[i] == 60 || tmp_path[i] == 62 || tmp_path[i] == 96)
                        r_error = "400 Bad Request";
            }
        }

        
        Start_line &get_start_line(){return start_line;}
        std::map<std::string,std::string> &get_header(){return header;}
        std::vector<std::string> &get_body(){return body;}
        std::string &get_body1(){return body1;}
        std::string &get_error(){return r_error;}

};


#endif