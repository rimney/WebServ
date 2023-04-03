#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include "lexer.hpp"
#include "parsing.hpp"

struct Start_line
{
    std::string method;
    std::string path;
    std::string vertion;
    std::string full_path;
    int location_index;
    std::string query;
};

class Request 
{

    private:
        Start_line start_line;
        std::string body;
        std::map<std::string,std::string> header;
        std::string r_error;
        bool         wait_body;
        unsigned long          body_size;
    public:
        Request(): wait_body(false),body_size(0){}
        Request(Request const & r)
        {*this = r;}
        Request & operator=(Request const & r)
        {
            this->start_line.method = r.start_line.method;
            this->start_line.path = r.start_line.path;
            this->start_line.full_path = r.start_line.full_path;
            this->start_line.vertion = r.start_line.vertion;
            this->start_line.location_index = r.start_line.location_index;
            this->start_line.query = r.start_line.query;
            this->body = r.body;
            this->header = r.header;
            this->r_error = r.r_error;
            this->body_size = r.body_size;
            this->wait_body = r.wait_body;
            return (*this); 
        }
        ~Request(){}

        void parser(std::string &value);
        void body_handling(std::string buffer);
        void errors(server_parser &serv);
        void request_well_formed(server_parser &serv);
        void location_well(server_parser &serv);
        void clear();
        Start_line &get_start_line(){return start_line;}
        std::map<std::string,std::string> &get_header(){return header;}
        std::string &get_body(){return body;}
        std::string &get_error(){return r_error;}
        bool &get_wait_body(){return wait_body;}
};


#endif