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
     * If it is not a part of the bit mask then the exiting_character is set to the failing character
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
};
#endif
