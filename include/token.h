#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>
#include "posinfo.h"

class Token
{
public:
    Token(int type, PosInfo posInfo);
    virtual ~Token();
    void setType(int type);
    int getType();
    void setValue(std::string svalue);
    std::string getValue();

    bool isString();
    bool isString(std::string value);
    bool isSymbol();
    bool isSymbol(std::string value);
    bool isTerminatingSymbol();
    bool isCastableType();
    bool isIdentifier();
    bool isIdentifier(std::string value);
    bool isKeyword();
    bool isKeyword(std::string value);
    bool isOperator();
    bool isLogicalOperator();
    bool isOperator(std::string value);
    bool isNumber();
    bool isNumber(std::string value);
    bool isLiteral();
    bool isBooleanLiteral();
    bool isDataAccessKeyword();

    // The next token after this one
    Token* next;
    int type;
    std::string value;
    
    PosInfo posInfo;
private:
};
#endif
