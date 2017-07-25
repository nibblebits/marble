#ifndef TOKEN_H
#define TOKEN_H
#include <string>

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
	private:
		int type;
		struct token_value value;
		
};
#endif
