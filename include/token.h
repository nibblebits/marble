#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>

#include "value.h"
class Token
{
    public:
        Token(int type);
        virtual ~Token();
		void setType(int type);
		int getType();
		void setValue(std::string svalue);
		struct value getValue();

		bool isString();
		bool isString(std::string value);
		bool isSymbol();
		bool isSymbol(std::string value);
		bool isIdentifier();
		bool isIdentifier(std::string value);
		bool isKeyword();
		bool isKeyword(std::string value);
		bool isOperator();
		bool isOperator(std::string value);
		bool isNumber();
		bool isNumber(std::string value);
		bool isLiteral();

		// The next token after this one
		std::shared_ptr<Token> next;
		int type;
		struct value value;
		
};
#endif
