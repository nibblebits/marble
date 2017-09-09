#include "lexer.h"
#include <vector>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <memory>
#include "statics.h"
#include "config.h"
const char keywords[][MAX_KEYWORD_SIZE] = {"public", "private", "protected", "number", "char", "string", "bool", "int", "class", "return", "continue", "break", "void", "if"};
const char valid_operators[][MAX_OPERATORS_SIZE] = {"+", "-", "*", "/", "++", "--", "+=", "-=", "/=", "*=", "-=", "=", ".", "&", "|", "!", "==", "!=", ">=", ">", "<=", "<", "&&", "||"};
const char symbols[] = {';',',','(', ')', '{', '}','[',']'};
Lexer::Lexer()
{
    this->root = NULL;
}
Lexer::~Lexer()
{

}

void Lexer::setInput(const char* buf, int size)
{
    this->buf = buf;
    this->size = size;
    this->end = this->buf+this->size;
}

bool Lexer::bounds_safe(const char* ptr)
{
    return ptr < this->end;
}

bool Lexer::is_keyword(std::string value)
{
    int total_keywords = sizeof(keywords) / MAX_KEYWORD_SIZE;
    for (int i = 0; i < total_keywords; i++)
    {
        const char* keyword = keywords[i];
        if (value == keyword)
        {
            return true;
        }
    }

    return false;
}

bool Lexer::is_whitespace(char c)
{
    return c < 33;
}

bool Lexer::is_operator(char c)
{
    std::string c_str = "";
    c_str += c;
    return is_operator(c_str);
}

bool Lexer::is_operator(std::string str)
{
    int total_operators = sizeof(valid_operators) / MAX_OPERATORS_SIZE;
    for (int i = 0; i < total_operators; i++)
    {
        const char* op = valid_operators[i];
        if(str == op)
        {
            return true;
        }
    }

    return false;
}

bool Lexer::is_number(char c)
{
    return c >= 48 && c <= 57;
}

bool Lexer::is_number(std::string number)
{
    for (int i = 0; i < number.size(); i++)
    {
        char c = number.at(i);
        if (!is_number(c))
            return false;
    }

    return true;
}

bool Lexer::is_string_seperator(char c)
{
    return c == '"';
}


bool Lexer::is_symbol(char c)
{
    int total_symbols = sizeof(symbols);
    for (int i = 0; i < total_symbols; i++)
    {
        if (c == symbols[i])
        {
            return true;
        }
    }

    return false;
}

int Lexer::is_character(char c)
{
    return !is_operator(c) && !is_symbol(c) && !is_number(c) && !is_whitespace(c);
}

bool Lexer::is_stackable(int token_type)
{
    return token_type == TOKEN_TYPE_OPERATOR || token_type == TOKEN_TYPE_NUMBER || token_type == TOKEN_TYPE_STRING;
}

int Lexer::get_type_of_char(char c)
{
    int type = -1;
    if (is_character(c))
    {
        type = IS_CHARACTER;
    }
    else if(is_operator(c))
    {
        type = IS_OPERATOR;
    }
    else if(is_number(c))
    {
        type = IS_NUMBER;
    }
    else if(is_whitespace(c))
    {
        type = IS_WHITESPACE;
    }
    else if(is_symbol(c))
    {
        type = IS_SYMBOL;
    }
    else
    {
        throw std::logic_error("Unexpected type for character " + std::to_string(c) + " in method get_type_of_char");
    }

    return type;
}

std::string Lexer::get_operator(const char** ptr)
{
    const char* our_ptr = *ptr;
    char c = *our_ptr;
    std::string value = "";
    while(is_operator(c))
    {
        value += c;
        our_ptr++;
        c = *our_ptr;
    }

    if (value != "")
    {
        *ptr += value.length();
    }
    return value;
}

std::string Lexer::get_number(const char** ptr)
{
    const char* our_ptr = *ptr;
    char c = *our_ptr;
    std::string value = "";
    while(is_number(c))
    {
        value += c;
        our_ptr++;
        c = *our_ptr;
    }

    if (value != "")
    {
        *ptr += value.length();
    }
    return value;
}

std::string Lexer::get_string(const char** ptr)
{
    if (!is_string_seperator(**ptr))
    {
        throw std::logic_error("Expecting a string seperator for a string");
    }

    // Ok this appears to be a valid string so far, lets move the pointer forward to ignore the string seperator
    *ptr+=1;
    // our_ptr will now point at the start of the string
    const char* our_ptr = *ptr;
    char c = *our_ptr;
    std::string value = "";

    // Lets loop until we find an ending string seperator.
    while(!is_string_seperator(c))
    {
        value += c;
        our_ptr++;
        c = *our_ptr;
    }


    // Lets re-adjust the real pointer for the caller, we +1 due to the ending string seperator.
    *ptr += value.length();
    return value;
}


std::string Lexer::get_while(const char** ptr, int expected)
{
    std::string tokenValue = "";
    char c = **ptr;
    int type = get_type_of_char(c);
    if (type != expected)
    {
        throw std::logic_error("While calling \"get_while\" the first character must be of the expected type");
    }
    while(*ptr < this->end)
    {
        c = **ptr;
        type = get_type_of_char(c);
        if (type != expected)
        {
            // Numbers can have decimal points which is seen as an operator
            if (expected == IS_NUMBER && type == IS_OPERATOR && c == '.')
            {
                // This is a floating point number so allow it
                goto next;
            }
            // Restore the pointer to previous state
            *ptr-=1;
            break;
        }
next:
        tokenValue += c;
        *ptr+=1;
    }

    return tokenValue;
}

/**
 * Stage 1 will remove all comments, and create tokens based on the input
*/
Token* Lexer::stage1()
{
    Token* root_token = NULL;
    Token* last_token = NULL;
    const char* ptr = this->buf;
    int token_type = -1;
    std::string token_value = "";
    // We will loop through the whole thing and when we reach a whitespace a token has been completed
    while (ptr < this->end)
    {
        char c = *ptr;
        if (is_whitespace(c))
        {
            ptr++;
            continue;
        }

        if (is_string_seperator(c))
        {
            token_type = TOKEN_TYPE_STRING;
            token_value = get_string(&ptr);
        }
        else
        {
            int c_type = get_type_of_char(c);
            switch(c_type)
            {
                case IS_OPERATOR:
                    token_type = TOKEN_TYPE_OPERATOR;
                    token_value = get_while(&ptr, IS_OPERATOR);
                break;
                case IS_CHARACTER:
                    token_value = get_while(&ptr, IS_CHARACTER);
                    if (is_keyword(token_value))
                    {
                        token_type = TOKEN_TYPE_KEYWORD;
                    }
                    else
                    {
                        token_type = TOKEN_TYPE_IDENTIFIER;
                    }
                break;
                case IS_NUMBER:
                    token_type = TOKEN_TYPE_NUMBER;
                    token_value = get_while(&ptr, IS_NUMBER);
                break;
                case IS_SYMBOL:
                    token_type = TOKEN_TYPE_SYMBOL;
                    token_value += c;
                break;
            }
        }

        Token* new_token = tokenFactory.createToken(token_type);
        new_token->setValue(token_value);
        if (root_token == NULL)
        {
            root_token = new_token;
            last_token = new_token;
        }
        else
        {
            last_token->next = new_token;
            last_token = new_token;
        }
        
        token_type = -1;
        token_value = "";
        // Increment the pointer
        ptr++;
    }

    return root_token;
}

/**
 * Stage 2 will ensure that the tokens values are valid.
 * Such as a stacked value is correct, e.g a stacked operator value "++--" is clearly illegal.
*/
void Lexer::stage2(Token* root_token)
{
    Token* token = root_token;
    while(token != NULL)
    {
        int token_type = token->getType();
        std::string token_value = token->getValue();
        switch(token_type)
        {
        case TOKEN_TYPE_OPERATOR:
        {
            if (!is_operator(token_value))
            {
                throw std::logic_error("Invalid operator: " + token_value);
            }
        }
        break;
        }

        token = token->next;
    }
}

Token* Lexer::lex()
{
    // Stage 1 - Remove comments; Create tokens
    this->root = stage1();
    // Stage 2 - Identify errors such as illegal operators
    stage2(this->root);
    return this->root;
}
