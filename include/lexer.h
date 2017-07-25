#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <memory>
#include "token.h"
class Lexer
{
    public:
	Lexer();
	virtual ~Lexer();
        void setInput(const char* buf, int size);
        const std::vector<std::shared_ptr<Token>> lex();
	
    private:
        const char* buf;
	const char* end;
	int size;
	
	void stage1(std::vector<std::shared_ptr<Token>>* tokens);
	void stage2(std::vector<std::shared_ptr<Token>>* tokens);
	bool bounds_safe(const char* ptr);
	bool is_keyword(std::string value);
	bool is_whitespace(char c);

	Token process_keyword(const char** ptr);
	Token process_identifier(const char** identifier);
};
#endif
