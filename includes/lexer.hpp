#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <string>
#include <vector>

enum type 
{
    TYPE_TEXT,
    TYPE_END_OF_LINE,
    TYPE_CR,
    TYPE_END_OF_SSECTION,
    TYPE_TWO_POINT,
    TYPE_EOF
};

struct token 
{
    std::string value;
    int type;
};

class lexer 
{
    private:
        const std::string& src;
        const unsigned int size;
        unsigned int index;
        bool is_key;
        token _token;

    public:
    lexer(const std::string& val)
        : src(val), size(val.length()), index(0), is_key(false)
    {
        _token.type = TYPE_EOF;
    }

    const std::string& get_src() const
    {
        return src;
    }

    int get_index(){return index;}

    token get_next_token(int is_line)
    {
        _token.value.clear();
        if (index >= size) {
            _token.type = TYPE_EOF;
            return _token;
        }
        while (index < size) {
            if ((src[index] == '\n' && (index + 1 < size && src[index + 1] == '\n'))  || (src[index] == 13 && (index + 1 < size && src[index + 1] == 13))) {
                _token.type = TYPE_END_OF_SSECTION;
                index += 2;
                break;
            } if ((src[index] == 13 && (index + 1 < size && src[index + 1] == 10)) &&  (index + 2 < size && src[index + 2] == 13) && (index + 3 < size && src[index + 3] == 10)) {
                _token.type = TYPE_END_OF_SSECTION;
                index += 4;
                break;
            }else if (src[index] == '\n') {
                _token.type = TYPE_END_OF_LINE;
                _token.value += src[index];
                is_key = false;
                ++index;
                break;
            } else if (src[index] == 13) {
                _token.type = TYPE_CR;
                _token.value += src[index];
                is_key = false;
                ++index;
                break;
            } else if ((src[index] == '\t' || src[index] == 32) && !is_key) {
                while (src[index] == '\t' || src[index] == 32)
                    ++index;
                break;
            } else if (src[index] == ':' && !is_key && is_line == 1) {
                _token.type = TYPE_TWO_POINT;
                _token.value += src[index];
                is_key = true;
                ++index;
                break;
            } else {
                _token.value += src[index];
                _token.type = TYPE_TEXT;
            }
            index++;
            if (src[index] == 13 || src[index] == '\n' || (src[index] == ':' && !is_key && is_line == 1) 
                        || (src[index] == '\n' && (index + 1 < size && src[index + 1] == '\n'))
                        || (src[index] == 13 && (index + 1 < size && src[index + 1] == 13)))
                break;
        }
        return _token;
    }
};

#endif