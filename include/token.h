#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>
class Token
{
    public:
        Token(int type);
        virtual ~Token();
		void setType(int type);
		int getType();
		void setValue(std::string svalue);
		std::string getValue();
		void setNext(Token* next_token);

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
		Token* next;
		int type;
		std::string value;
	private:
		// A unique_ptr that points to the same address as "next"
		std::unique_ptr<Token> smart_next;
};
#endif
