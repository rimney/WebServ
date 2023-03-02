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
    public:
        token           _token;
        lexer() : size(0),index(0)
        {
            _token._type =  TYPE_EOF;
        }
        lexer(std::string const &val)
        {
            src = val;
            size = val.length();
            index = 0;
        }
        ~lexer(){}

        std::string get_src(){return src;}

        token	get_next_token()
        {
            // while ((src[index] <= size) && (src[index]  == '\n'
            //     || src[index]  == '\t' || src[index]  == 32))
            //     index++;
            _token.value.clear();
            while(index < size)
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
                    index++;
                    break;
                }
                else if(src[index]  == '\t' || src[index]  == 32)
                {
                    index++;
                    break;
                }
                else if (src[index]  == ':')
                {
                    _token._type = TYPE_TWO_POINT;
                    index++;
                    break;
                }
                else
                {
                    _token.value.push_back(src[index]);
                    _token._type = TYPE_TEXT;
                }
                index++;
                if(src[index] == '\n')
                    break;
            }
            if (index >= size)
                _token._type = TYPE_EOF;
            return (_token);
        }
};




#endif