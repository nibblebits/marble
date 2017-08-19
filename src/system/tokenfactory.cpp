#include "tokenfactory.h"
#include "token.h"
TokenFactory::TokenFactory()
{

}
TokenFactory::~TokenFactory()
{

}

Token* TokenFactory::createToken(TOKEN_TYPE token_type)
{
    Token* token;
    switch(token_type)
    {
    case TOKEN_TYPE_NUMBER:
    {
        token = new Token(TOKEN_TYPE_NUMBER);
    }
    break;

    case TOKEN_TYPE_OPERATOR:
    {
        token = new Token(TOKEN_TYPE_OPERATOR);
    }
    break;

    case TOKEN_TYPE_STRING:
    {
        token = new Token(TOKEN_TYPE_STRING);
    }
    break;

    case TOKEN_TYPE_IDENTIFIER:
    {
        token = new Token(TOKEN_TYPE_IDENTIFIER);
    }
    break;

    case TOKEN_TYPE_KEYWORD:
    {
        token = new Token(TOKEN_TYPE_KEYWORD);
    }
    break;

    case TOKEN_TYPE_SYMBOL:
    {
        token = new Token(TOKEN_TYPE_SYMBOL);
    }
    break;

    default:
        throw std::logic_error("Invalid token type provided to TokenFactory::createToken");
    }

    // Lets create a unique pointer and add it to the nodes vector so the memory is freed when the node factory leaves its scope.
    this->tokens.push_back(std::unique_ptr<Token>(token));
    return token;
}
