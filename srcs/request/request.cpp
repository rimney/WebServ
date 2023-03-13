#include "/Users/brmohamm/Desktop/WebServ/includes/request.hpp"
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
                                & buffer[buffer.length() - 4] == 13 && buffer[buffer.length() - 5] == '0' && buffer[buffer.length() - 6] == 10 && buffer[buffer.length() - 7] == 13)
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
                                body_size = std::stoul(hexa, nullptr, 16);
                                i = j + 2;
                                break;
                            }
                            hexa += buffer[j];
                            if((buffer[j] < 48 || buffer[j] > 57) &&  (buffer[j] < 65 || buffer[j] > 70) )
                                        break;
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
    std::string tmp_path = start_line.path;
    if(!header.find("Transfer-Encoding")->first.empty())
    {
        if(header.find("Transfer-Encoding")->second != "chunked")
            r_error = "501 Not Implemented";
    }
    if((start_line.method == "POST" && header.find("Transfer-Encoding")->first.empty() && header.find("Content-Length")->first.empty())
        || (start_line.method == "POST" && !header.find("Transfer-Encoding")->first.empty() && !header.find("Content-Length")->first.empty()))
        r_error = "400 Bad Request";
    if(!header.find("Content-Length")->first.empty())
    {
        if(serv.getCmbsObject()  <  stoi(header.find("Content-Length")->second))
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
void Request::clear()
{
    start_line.method.clear();
    start_line.path.clear();
    start_line.vertion.clear();
    body.clear();
    header.clear();
    r_error.clear();
}
      