#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>

struct token_value
{
	// By default all numbers are doubles
	double dvalue;
	std::string svalue;
};

class Token
{
    public:
        Token(int type);
        virtual ~Token();
		void setType(int type);
		int getType();
		void setValue(std::string svalue);
		struct token_value getValue();

		// The next token after this one
		std::shared_ptr<Token> next;
	private:
		int type;
		struct token_value value;
		
};
#endif
