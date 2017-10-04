#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <string>
#include "scope.h"
#include "statics.h"
#include "scopehandler.h"
class Logger;
class Node;
class Validator : public ScopeHandler
{
public:
    Validator(Logger* logger);
    virtual ~Validator();
    void validate(Node* root_node);
    void expecting(VALUE_TYPE type);
    void expectingObject(std::string obj_name);
    bool isExpecting();
    void endExpecting();
    VALUE_TYPE getExpectingType();
    std::string getExpectingObject();
private:
    Logger* logger;
    VALUE_TYPE expecting_type;
    std::string expecting_object;
};

#endif
