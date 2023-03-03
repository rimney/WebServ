#ifndef LEXER_HPP
#define LEXER_HPP
#include <iostream>
#include <string>
#include <vector>


struct token 
{
    std::string	value;
    int _type;
};
enum type
{
    TYPE_TEXT , 
    TYPE_END_OF_LINE,
    TYPE_END_OF_SSECTION,
    TYPE_TWO_POINT,
    TYPE_EOF
};

class lexer
{
    private:
        
        std::string		src;
        unsigned int	size;
        unsigned int	index;
        bool            is_key;
    public:
        token           _token;
        lexer() : size(0),index(0)
        {
            _token._type =  TYPE_EOF;
            is_key = false;
        }
        lexer(std::string const &val)
        {
            src = val;
            size = val.length();
            index = 0;
            is_key = false;
        }
        ~lexer(){}

        std::string get_src(){return src;}

        token	get_next_token()
        {
            _token.value.clear();
            if (index  >= size)
            {
                _token._type = TYPE_EOF;
                return (_token);
            }
            while(index <= size)
            {
                if ( src[index]  == '\n' && src[index + 1] == '\n')
                {
                    _token._type = TYPE_END_OF_SSECTION;
                    index += 2;
                    break;
                }
                else if (src[index]  == '\n')
                {
                    _token._type = TYPE_END_OF_LINE;
                    _token.value.push_back(src[index]);
                    is_key = false;
                    index++;
                    break;
                }
                else if(src[index]  == '\t' || src[index]  == 32 && is_key == false)
                {
                    while(src[index]  == '\t' || src[index]  == 32 )
                        index++;
                    break;
                }
                else if (src[index]  == ':' && is_key == false)
                {
                    _token._type = TYPE_TWO_POINT;
                    _token.value.push_back(src[index]);
                    is_key = true;
                    index++;
                    break;
                }
                else 
                {
                    if (src[index]  == ':'  && src[index]  == '\n')
                        index--;
                    else
                    {
                        _token.value.push_back(src[index]);
                        _token._type = TYPE_TEXT;
                    }
                }
                index++;
                if(src[index] == '\n' || (src[index]  == ':' && is_key == false ) 
                        || (src[index]  == '\n' && src[index + 1] == '\n'))
                    break;
            }
            return (_token);
        }
};



// GET /favicon.ico HTTP/1.1
// Host: localhost:8080
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:109.0) Gecko/20100101 Firefox/110.0
// Accept: image/avif,image/webp,*/*
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
// Referer: http://localhost:8080/
// Sec-Fetch-Dest: image
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Site: same-origin

// POST /api/ValidateTwitterFollowerCount HTTP/1.1
// Host: myazurefunction.azurewebsites.net
// Content-Type: application/json
// cache-control: no-cache
// Postman-Token: XXXXXXX-XXXXX-XXXXXX

// {
//     "followersCount" : 220,
//     "tweettext":"#Stack Overflow rocks",
//     "Name": "John Doe"
// }
#endif