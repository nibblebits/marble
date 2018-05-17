#ifndef TOKENFACTORY_H
#define TOKENFACTORY_H
#include <memory>
#include <vector>
#include "statics.h"
#include "posinfo.h"
/**
* This token factory will handle the creation of all tokens. Memory will be freed once the node factory leaves scope. Ensure that creation of tokens
* is not done from out of this class
*/

class Token;
class TokenFactory
{
public:
    TokenFactory();
    virtual ~TokenFactory();
    Token* createToken(TOKEN_TYPE token_type, PosInfo posInfo);
private:
    std::vector<std::unique_ptr<Token>> tokens;
};

#endif
