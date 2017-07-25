#include "token.h"
#include <stdlib.h>
Token::Token(int type)
{
    this->type = type;
}

Token::~Token()
{

}

void Token::setType(int type)
{
	this->type = type;
}

int Token::getType()
{
	return this->type;
}

void Token::setValue(std::string svalue)
{
	this->value.dvalue = atof(svalue.c_str());
	this->value.svalue = svalue;
}

struct token_value Token::getValue()
{
	return this->value;
}
