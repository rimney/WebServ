#include "../../includes/request.hpp"

void Request::parser(std::string &value)
{
    token _token;
    lexer _lexer(value);
    std::string buffer;
    bool end_of_section = false;

    uintptr_t p = reinterpret_cast<uintptr_t>(&start_line);
    std::string *t = reinterpret_cast<std::string *>(p);
    for(int i = 0 ;_token.type != TYPE_EOF;)
    {
        if(i == 0)
            _token = _lexer.get_next_token(0);
        else
            _token = _lexer.get_next_token(1);
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
            _lexer.get_next_token(1);
            buffer = _lexer.get_next_token(1).value;
            buffer.erase(buffer.begin());
            if(!buffer.empty() && !_token.value.empty())
            header.insert(std::make_pair(_token.value,buffer));
            buffer.clear();
        }
        else if(_token.type == TYPE_END_OF_SSECTION)
        {
            buffer.clear();
            for (int i = _lexer.get_index(); i < (int)value.length(); i++)
                buffer += value[i];
            body = buffer;
            break;
        }
        _token.value.clear();
    }

    if(header.find("Transfer-Encoding") != header.end())
    {
        if(header["Transfer-Encoding"] == "chunked" )
        {
            wait_body = true;
            body_handling(body);
        }
    }
    else if (header.find("Content-Length") != header.end())
    {
        wait_body = true;
        body_size = atol(header["Content-Length"].c_str());
        if((unsigned long)body.length() >= body_size)
            wait_body = false;
    }
    else if(start_line.method == "POST" && header.find("Content-Length") != header.end() && header.find("Transfer-Encoding") != header.end())
        wait_body = true;
}

void Request::body_handling(std::string buffer)
{
    std::string hexa;

    if(header.find("Transfer-Encoding") != header.end())
    {
        if(buffer[buffer.length() - 1] == 10 && buffer[buffer.length() - 2] == 13 && buffer[buffer.length() - 3] == 10
                                && buffer[buffer.length() - 4] == 13 && buffer[buffer.length() - 5] == '0' && buffer[buffer.length() - 6] == 10 && buffer[buffer.length() - 7] == 13)
        {
        
            buffer.erase(buffer.length() - 7 , buffer.length() - 1);
            wait_body = false;
        }
        if(header["Transfer-Encoding"] == "chunked" )
        {
            if(body_size == 0)
            {
                for(long i = 0 ; i < (long)buffer.length(); i++)
                {
                    if(buffer[i] == 13 && buffer[i + 1] == 10)
                    {
                        buffer.erase(0,i + 2);
                        break;
                    }
                    else
                        hexa += buffer[i];
                }
                std::cerr << "*"<< hexa << "*"<< std::endl;
                body_size = std::stoul(hexa, nullptr, 16);
                body = buffer;
            }
            else
            {
                
                for (int  i = 0; i < (int)buffer.length() ; i++)
                {
                    if(buffer[i] == 13 && buffer[i + 1] == 10)
                    {
                        for(int j = i + 2 ; j < (int)buffer.length() ; j++)
                        {
                            if(buffer[j] == 13 && buffer[j + 1] == 10)
                            {

                                if(!hexa.empty())
                                {
                                    std::cerr << hexa << std::endl;
                                    body_size = std::stoul(hexa, nullptr, 16);
                                    i = j + 2;
                                    break;
                                }
                            }
                            hexa += buffer[j];
                            if((buffer[j] < 48 || buffer[j] > 57) &&  (buffer[j] < 65 || buffer[j] > 70) )
                            {
                                hexa.clear();
                                break;
                            }
                        }
                    }
                    body += buffer[i];
                }
            }
        }
    }
    else if(header.find("Content-Length") != header.end())
    {
        body += buffer;
        if((unsigned long)body.length() >= body_size)
            wait_body = false;
    }
}

void Request::errors(server_parser &serv)
{
    request_well_formed(serv);
    if(r_error == "")
        location_well(serv);
}

void Request::location_well(server_parser &serv)
{
    size_t found = 0;
    int query_pos = 0;
    long index = start_line.location_index = -1;
    size_t index_of_charachter = 0;
    bool method_allowed = false;

    for(size_t i = 0 ; i < (size_t)serv.getServerLocationsObject().size(); i++)
    {
        found = start_line.path.find(serv.getServerLocationsObject()[i].getLocationNameObject());
        if (found != (size_t) -1  && index_of_charachter < serv.getServerLocationsObject()[i].getLocationNameObject().length())
        {
            index_of_charachter = serv.getServerLocationsObject()[i].getLocationNameObject().length();
            index = (long)i;
        }
    }
    if(index != -1)
    {
        start_line.location_index = index;
        if(!serv.getServerLocationsObject()[index].getLocationRedirectionObject().empty()) //301 Moved Permanently
            r_error = "301";
        if(serv.getServerLocationsObject()[index].getLocationMethodsObject().size() > 0)//405 Method Not Allowed
        {
            for(size_t i = 0; i < serv.getServerLocationsObject()[index].getLocationMethodsObject().size() ; i++)
            {
                if(serv.getServerLocationsObject()[index].getLocationMethodsObject()[i] == start_line.method)
                    method_allowed = true;
            }
            if(!method_allowed)
                r_error = "405";
        }
        else if(start_line.method !=  "POST" && start_line.method !=  "DELETE" && start_line.method !=  "GET")
            r_error = "405";
        if(!serv.getServerLocationsObject()[index].getLocationRootObject().empty())//404 Not Found
        {
            if(serv.getServerLocationsObject()[index].getLocationRootObject() [serv.getServerLocationsObject()[index].getLocationRootObject() .length() - 1] == '/' && start_line.path[0] == '/')
                 start_line.path.erase(0,1);
            start_line.full_path = serv.getServerLocationsObject()[index].getLocationRootObject() + start_line.path;
        }
        else if(!serv.getRootObject().empty())
        {
            if(serv.getRootObject()[serv.getRootObject().length() - 1] == '/' && start_line.path[0] == '/')
                 start_line.path.erase(0);
            start_line.full_path = serv.getRootObject() + start_line.path;
        }
        else
        {
            r_error = "404";

        }
        if(!start_line.full_path.empty())//query
        {
            query_pos = start_line.full_path.find("?");
            if(query_pos != -1)
            {
                for(int i = query_pos +1 ; i < (int)start_line.full_path.length() && start_line.full_path[i] != '#'   ; i++)
                    start_line.query += start_line.full_path[i];
                start_line.full_path.erase(query_pos);
                query_pos = start_line.path.find("?");
                start_line.path.erase(query_pos);
            }
        }
        if(header.find("Content-Type") != header.end() && header["Content-Type"] == "application/x-www-form-urlencoded")//query from post
            start_line.query = body;
    }
    else
    {
        r_error = "404";
        for(size_t i = 0 ; i < (size_t)serv.getServerLocationsObject().size(); i++)
        {
            found = start_line.path.find(serv.getServerLocationsObject()[i].getLocationNameObject());
            if (found != (size_t) -1  && index_of_charachter <= serv.getServerLocationsObject()[i].getLocationNameObject().length())
            {
                index_of_charachter = serv.getServerLocationsObject()[i].getLocationNameObject().length();
                index = (long)i;
            }
        }
    }
        
}

int alowed_character(char c)
{
    std::string valid = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    for(int i = 0 ; i < (int)valid.length();i++)
    {
        if(c == valid[i])
            return 1;
    }
    return 0;
}

int chek_hex(char c)
{
    std::string valid = "ABCDEF0123456789";
    for(int i = 0 ; i < (int)valid.length();i++)
    {
        if(c == valid[i])
            return 1;
    }
    return 0;
}

void Request::request_well_formed(server_parser &serv)
{
    std::string tmp_path = start_line.path;
    if(header.find("Transfer-Encoding") != header.end())
    {
        if(header["Transfer-Encoding"] != "chunked")
        {
            r_error = "501 Not Implemented";
            return;
        }
            
    }
    else if((start_line.method == "POST" && header.find("Transfer-Encoding") == header.end() && header.find("Content-Length") == header.end())
        || (start_line.method == "POST" && header.find("Transfer-Encoding") != header.end() && header.find("Content-Length") != header.end()))
        {
            r_error = "400";
            return;
        }
        
    if(body.size() > 0)
    {
        if(serv.getCmbsObject()  > 0 && (size_t)serv.getCmbsObject()  <  body.size())
        {
            r_error = "413 Request Entity Too Large";
            return;
        }
    }
    if(tmp_path.length() > 2048)
        r_error = "414 Request-URI Too Long";
    for(int i = 0 ; i < (int)tmp_path.length() ; i++)
    {
        if(alowed_character(tmp_path[i]) == 0)
        {
            r_error = "400";
            break;
        }
        if(tmp_path[i] == '%' && i + 1  < (int)tmp_path.length() && i + 2  < (int)tmp_path.length())
        {
            if( chek_hex(tmp_path[i + 1]) == 1 && chek_hex(tmp_path[i + 2]) == 1)
            {
                r_error = "400";
                break;
            } 
        }
    }
}

void Request::clear()
{
    start_line.method.clear();
    start_line.path.clear();
    start_line.vertion.clear();
    start_line.full_path.clear();
    start_line.location_index = -1;
    start_line.query.clear();
    wait_body = false;
    body.clear();
    header.clear();
    r_error.clear();
}
      