#include "../../includes/request.hpp"

void Request::parser(std::string value)
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

    if(body.empty())
        wait_body = false;
    else
    {
            
        if(!header.find("Transfer-Encoding")->first.empty())
        {
            if(header.find("Transfer-Encoding")->second == "chunked" )
            {
                wait_body = true;
                body_handling(body);
            }
        }
        else if (!header.find("Content-Length")->first.empty())
        {
            wait_body = true;
            body_size = atol(header.find("Content-Length")->second.c_str());
            if((unsigned long)body.length() >= body_size)
                wait_body = false;
        }  
    }
}

void Request::body_handling(std::string buffer)
{
    std::string hexa;

    if(!header.find("Transfer-Encoding")->first.empty())
    {
        if(buffer[buffer.length() - 1] == 10 && buffer[buffer.length() - 2] == 13 && buffer[buffer.length() - 3] == 10
                                && buffer[buffer.length() - 4] == 13 && buffer[buffer.length() - 5] == '0' && buffer[buffer.length() - 6] == 10 && buffer[buffer.length() - 7] == 13)
        {
        
            buffer.erase(buffer.length() - 7 , buffer.length() - 1);
            wait_body = false;
        }
        if(header.find("Transfer-Encoding")->second == "chunked" )
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
                body_size = std::strtoul(hexa.c_str(), 0, 16);//hexa
                body = buffer;
            }
            else
            {
                buffer_body.clear();
                for(int i = body.length() - 20 ;i < (int)(body.length()); i++)
                {
                    buffer_body += body[i];
                }
                body.erase(body.length() - 20);
                buffer.insert(0,buffer_body);
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
                                    body_size = std::strtoul(hexa.c_str(), 0, 16);
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
    else if(!header.find("Content-Length")->first.empty())
    {
        body += buffer;
        if((unsigned long)body.length() >= body_size)
            wait_body = false;
    }
}

void Request::errors(server_parser &serv)
{
    request_well_formed(serv);
    location_well(serv);
}

void Request::location_well(server_parser &serv)
{
    size_t found = 0;
    int query_pos = 0;
    long index = start_line.location_index = -1;
    size_t index_of_charachter = 0;
    bool method_allowed = false;
    for(size_t i = 0 ; i < (size_t)serv.getServerLocationsObject().size(); i++)//serche path
    {
        found = start_line.path.find(serv.getServerLocationsObject()[i].getLocationNameObject());
        if (found != (size_t) -1  && index_of_charachter <= serv.getServerLocationsObject()[i].getLocationNameObject().length())
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
        if(!serv.getServerLocationsObject()[index].getLocationMethodsObject().empty())//405 Method Not Allowed
        {
            for(size_t i = 0; i < serv.getServerLocationsObject()[index].getLocationMethodsObject().size() ; i++)
            {
                if(serv.getServerLocationsObject()[index].getLocationMethodsObject()[i] == start_line.method)
                    method_allowed = true;
            }
            if(!method_allowed)
                r_error = "405";
        }
        if(!serv.getServerLocationsObject()[index].getLocationRootObject().empty())//404 Not Found
            start_line.full_path = serv.getServerLocationsObject()[index].getLocationRootObject() + start_line.path;
        else if(!serv.getRootObject().empty())
        {
            start_line.full_path = serv.getRootObject() + start_line.path;
        }
        else
            r_error = "404";
        if(!start_line.full_path.empty())//query
        {
            query_pos = start_line.full_path.find("?");
            if(query_pos != -1)
            {
                for(int i = query_pos +1 ; i < (int)start_line.full_path.length() && start_line.full_path[i] != '#'   ; i++)
                    start_line.query += start_line.full_path[i];
            }
        }
    }
    else
        r_error = "404";
   
}

void Request::request_well_formed(server_parser &serv)
{
    std::string tmp_path = start_line.path;
    if(!header.find("Transfer-Encoding")->first.empty())
    {
        if(header.find("Transfer-Encoding")->second != "chunked")
            r_error = "501 Not Implemented";
    }
    else if((start_line.method == "POST" && header.find("Transfer-Encoding")->first.empty() && header.find("Content-Length")->first.empty())
        || (start_line.method == "POST" && !header.find("Transfer-Encoding")->first.empty() && !header.find("Content-Length")->first.empty()))
        r_error = "400 Bad Request";
    else if(!header.find("Content-Length")->first.empty())
    {
        if(serv.getCmbsObject()  > 0 && serv.getCmbsObject()  <  stoi(header.find("Content-Length")->second))
            r_error = "413 Request Entity Too Large";
    }
    else if(tmp_path.length() > 2048)
        r_error = "414 Request-URI Too Long";
    for(int i = 0 ; i < (int)tmp_path.length() ; i++)
    {
        if(((tmp_path[i] < 95 || tmp_path[i] > 'z') && (tmp_path[i] < 33 || tmp_path[i] > 90))
            || tmp_path[i] == 34 || tmp_path[i] == 60 || tmp_path[i] == 62 || tmp_path[i] == 96)
        {
            r_error = "400 Bad Request";
            break;
        }
    }
}

void Request::clear()
{
    start_line.method.clear();
    start_line.path.clear();
    start_line.vertion.clear();
    body.clear();
    header.clear();
    r_error.clear();
}
      