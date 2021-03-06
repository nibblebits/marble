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

#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <memory>
#include "tokenfactory.h"
#include "token.h"
#include "logger.h"

class Lexer
{
public:
    Lexer(Logger* logger);
    virtual ~Lexer();
    void setInput(const char* buf, int size);
    Token* lex(PosInfo posInfo);

private:
    Token* root;
    const char* buf;
    const char* end;
    int size;

    Token* stage1(PosInfo posInfo);
    void stage2(Token* root_token);
    bool bounds_safe(const char* ptr);
    bool is_keyword(std::string value);
    bool is_whitespace(char c);
    bool is_operator(char c);
    bool is_operator(std::string str);
    bool is_number(char c);
    bool is_number(std::string number);
    bool is_string_seperator(char c);
    bool is_symbol(char c);
    bool is_comment(char c);
    int is_character(char c);
    bool is_stackable(int token_type);
    void error(std::string message, PosInfo posInfo);
    int get_type_of_char(char c, PosInfo& posInfo);
    std::string get_operator(const char** ptr);
    std::string get_number(const char** ptr, PosInfo& posInfo);
    std::string get_string(const char** ptr, PosInfo& posInfo);
    /**
     * Gets a string if the expected bit mask is valid for the given character read.
     * If it is not a part of the bit mask then the exiting_character is set to the failing character.
     * 
     * When this function returns the ptr points to the last valid character for this get_while call.
     * For example if you want to get a string of numbers "IS_NUMBER" and you have 53827+ then get_while will return
     * 53827 and **ptr will equal 7 as 7 was the last valid character
     */
    std::string get_while(const char** ptr, int expected, PosInfo& posInfo, int* exiting_character=NULL);
    void ignore_line(const char** ptr);
    /**
     * Returns a char based on the sequence for the given character
     * this is used in strings so you can do \r \n and so on to return new line char codes and such.
     */
    char get_char_for_sequence(char c);

private:
    TokenFactory tokenFactory;
    Logger* logger;
    const char* filename;
    /* Char sequences such as "/n" will be treated as "\n" and not a new line if this is set to true. This is for all char sequences
     * this is set to true automatically when prepending ":" before any string, for example :"Hello\nworld" */
    bool ignore_char_sequence;
};
#endif
