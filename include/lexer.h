#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <memory>
#include "tokenfactory.h"
#include "token.h"


class Lexer
{
public:
    Lexer();
    virtual ~Lexer();
    void setInput(const char* buf, int size);
    Token* lex();

private:
    Token* root;
    const char* buf;
    const char* end;
    int size;

    Token* stage1();
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
    int is_character(char c);
    bool is_stackable(int token_type);
    int get_type_of_char(char c);
    std::string get_operator(const char** ptr);
    std::string get_number(const char** ptr);
    std::string get_string(const char** ptr);
    std::string get_while(const char** ptr, int expected);

private:
    TokenFactory tokenFactory;
};
#endif
