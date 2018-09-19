
/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "lexer.h"
#include <vector>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <sstream>
#include <memory>
#include <iomanip>
#include <ctype.h>
#include "statics.h"
#include "config.h"
const char keywords[][MAX_KEYWORD_SIZE] = {"public", "private", "protected", "function", "number", "string", "int", "boolean", "true", "false", "class", "return", "continue", "break", "void", "new", "if", "else", "extends", "throw", "try", "catch", "finally", "do", "while", "break", "continue", "for", "include", "include_once", "pure", "final", "permission", "null", "require", "limit", "scope", "filter", "output", "operator"};
const char valid_operators[][MAX_OPERATORS_SIZE] = {"+", "-", "*", "/", "++", "--", "+=", "-=", "/=", "*=", "-=", "=", ".", "&", "|", "%", "^", "~", "!", "==", "!=", ">=", ">", "<=", "<", "&&", "||"};
const char symbols[] = {';', ',', '(', ')', '{', '}', '[', ']', ':', '@'};
Lexer::Lexer(Logger *logger)
{
    this->logger = logger;
    this->root = NULL;
}
Lexer::~Lexer()
{
}

void Lexer::setInput(const char *buf, int size)
{
    this->buf = buf;
    this->size = size;
    this->end = this->buf + this->size;
}

bool Lexer::bounds_safe(const char *ptr)
{
    return ptr < this->end;
}

bool Lexer::is_keyword(std::string value)
{
    int total_keywords = sizeof(keywords) / MAX_KEYWORD_SIZE;
    for (int i = 0; i < total_keywords; i++)
    {
        const char *keyword = keywords[i];
        if (value == keyword)
        {
            return true;
        }
    }

    return false;
}

bool Lexer::is_whitespace(char c)
{
    return isspace(c);
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
        const char *op = valid_operators[i];
        if (str == op)
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

bool Lexer::is_comment(char c)
{
    return c == '#';
}

int Lexer::is_character(char c)
{
    return !is_operator(c) && !is_symbol(c) && !is_number(c) && !is_whitespace(c);
}

bool Lexer::is_stackable(int token_type)
{
    return token_type == TOKEN_TYPE_OPERATOR || token_type == TOKEN_TYPE_NUMBER || token_type == TOKEN_TYPE_STRING;
}

void Lexer::error(std::string message, PosInfo posInfo)
{
    logger->error(message, posInfo);
}

int Lexer::get_type_of_char(char c, PosInfo &posInfo)
{
    int type = -1;
    if (is_character(c))
    {
        type = IS_CHARACTER;
    }
    else if (is_operator(c))
    {
        if (c == '~')
            error("The operator ~ has not yet been implemented in Marble as Marble is in beta testing there is a small lack of features that will exist in the stable. Please wait until the stable is released or if it is released upgrade to stable to use this operator", posInfo);

        type = IS_OPERATOR;
    }
    else if (is_number(c))
    {
        type = IS_NUMBER;
    }
    else if (is_whitespace(c))
    {
        type = IS_WHITESPACE;
    }
    else if (is_symbol(c))
    {
        type = IS_SYMBOL;
    }
    else
    {
        error("Unexpected type for character " + std::to_string(c) + " in method get_type_of_char", posInfo);
    }

    return type;
}

std::string Lexer::get_operator(const char **ptr)
{
    const char *our_ptr = *ptr;
    char c = *our_ptr;
    std::string value = "";
    while (is_operator(c))
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

std::string Lexer::get_number(const char **ptr, PosInfo &posInfo)
{
    int exiting_character = -1;
    std::string value = get_while(ptr, IS_NUMBER, posInfo, &exiting_character);
    if (exiting_character != -1 && value.size() == 1 && value[0] == '0')
    {
        // We may have a hexadecimal or binary number here as get_while terminated after 0 was found. Let's look ahead and see
        if (exiting_character == 'x')
        {
            // It looks like we have hexadecimal let's forward the pointer twice to ignore "0x"
            *ptr += 2;
            posInfo.col += 1;
            value = get_while(ptr, IS_NUMBER | IS_CHARACTER, posInfo);

            // Let's get the hex value
            int v;
            std::istringstream(value) >> std::hex >> v;
            // Finally set the value again to the decimal result of the hexadecimal
            value = std::to_string(v);
            return value;
        }
    }

    // If this number begins with zero and there is multiple numbers such as "02" or "0456" then it is octal and conversion should happen
    if (value.size() > 1)
    {
        if (value[0] == '0')
        {
            // Let's convert from octal to decimal and then set the value to this
            int v;
            std::istringstream(value) >> std::oct >> v;
            value = std::to_string(v);
        }
    }
    return value;
}

char Lexer::get_char_for_sequence(char c)
{
    char result = 0;
    switch (c)
    {
    case 'r':
        result = '\r';
        break;
    case 'n':
        result = '\n';
        break;
    case '0':
        result = '\0';
        break;
    case '"':
        result = '"';
        break;
    case '\\':
        result = '\\';
        break;

    default:
        logger->error("Bad char sequence for: " + c);
    }

    return result;
}

std::string Lexer::get_string(const char **ptr, PosInfo &posInfo)
{
    if (!is_string_seperator(**ptr))
    {
        error("Expecting a string seperator for a string", posInfo);
    }

    // Ok this appears to be a valid string so far, lets move the pointer forward to ignore the string seperator
    *ptr += 1;
    // our_ptr will now point at the start of the string
    const char *our_ptr = *ptr;
    char c = *our_ptr;
    int length = 0;
    std::string value = "";

    // Lets loop until we find an ending string seperator.
    while (bounds_safe(our_ptr) && !is_string_seperator(c))
    {
        if (c == '\\')
        {
            /* Some characters are valid in strings such as carriage returns and new lines \r\n
             * Let's handle it here*/
            our_ptr += 1;
            c = *our_ptr;
            c = get_char_for_sequence(c);
        }
        value += c;
        our_ptr += 1;
        c = *our_ptr;
    }

    // Adjust the main pointer to point to the new position
    *ptr += (our_ptr - *ptr);
    return value;
}

std::string Lexer::get_while(const char **ptr, int expected, PosInfo &posInfo, int *exiting_character)
{
    std::string tokenValue = "";
    char c = **ptr;
    int type = get_type_of_char(c, posInfo);
    if (!(type & expected))
    {
        error("While calling \"get_while\" the first character must be of the expected type", posInfo);
    }
    while (bounds_safe(*ptr))
    {
        c = **ptr;
        type = get_type_of_char(c, posInfo);
        if (!(type & expected))
        {
            // Numbers can have decimal points which is seen as an operator
            if (expected == IS_NUMBER && type == IS_OPERATOR && c == '.')
            {
                // This is a floating point number so allow it
                goto next;
            }
            // Restore the pointer to previous state
            *ptr -= 1;
            posInfo.col -= 1;
            if (exiting_character != NULL)
                *exiting_character = c;
            break;
        }
    next:
        tokenValue += c;
        *ptr += 1;
        posInfo.col += 1;
    }

    return tokenValue;
}

void Lexer::ignore_line(const char **ptr)
{
    while (bounds_safe(*ptr) && **ptr != 0x0a)
        *ptr += 1;
}

/**
 * Stage 1 will remove all comments, and create tokens based on the input
*/
Token *Lexer::stage1(PosInfo posInfo)
{
    Token *root_token = NULL;
    Token *last_token = NULL;
    const char *ptr = this->buf;
    int token_type = -1;
    std::string token_value = "";
    // We will loop through the whole thing and when we reach a whitespace a token has been completed
    while (bounds_safe(ptr))
    {
        char c = *ptr;
        if (is_whitespace(c))
        {
            if (c == 0x0a)
            {
                // New line.
                posInfo.line += 1;
                posInfo.col = 1;
            }
            else
            {
                posInfo.col += 1;
            }
            ptr += 1;
            continue;
        }

        if (is_string_seperator(c))
        {
            token_type = TOKEN_TYPE_STRING;
            token_value = get_string(&ptr, posInfo);
        }
        else if (is_comment(c))
        {
            ignore_line(&ptr);
            continue;
        }
        else
        {
            int c_type = get_type_of_char(c, posInfo);
            switch (c_type)
            {
            case IS_OPERATOR:
                token_type = TOKEN_TYPE_OPERATOR;
                token_value = get_while(&ptr, IS_OPERATOR, posInfo);
                if (token_value == "++")
                    error("Marble does not support unary \"++\". Please use \"+=1\"", posInfo);
                else if (token_value == "--")
                    error("Marble does not support unary operator \"--\". Please use \"-=1\"", posInfo);

                break;
            case IS_CHARACTER:
                token_value = get_while(&ptr, IS_CHARACTER | IS_NUMBER, posInfo);
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
                token_value = get_number(&ptr, posInfo);
                break;
            case IS_SYMBOL:
                token_type = TOKEN_TYPE_SYMBOL;
                token_value += c;
                break;
            }
        }

        Token *new_token = tokenFactory.createToken(token_type, posInfo);
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
        ptr += 1;
        posInfo.col += 1;
    }

    return root_token;
}

/**
 * Stage 2 will ensure that the tokens values are valid.
 * Such as a stacked value is correct, e.g a stacked operator value "++--" is clearly illegal.
*/
void Lexer::stage2(Token *root_token)
{
    Token *token = root_token;
    while (token != NULL)
    {
        int token_type = token->getType();
        std::string token_value = token->getValue();
        switch (token_type)
        {
        case TOKEN_TYPE_OPERATOR:
        {
            if (!is_operator(token_value))
            {
                error("Invalid operator: " + token_value, token->posInfo);
            }
        }
        break;
        }

        token = token->next;
    }
}

Token *Lexer::lex(PosInfo posInfo)
{
    // Stage 1 - Remove comments; Create tokens
    this->root = stage1(posInfo);
    // Stage 2 - Identify errors such as illegal operators
    stage2(this->root);
    return this->root;
}
