#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include "value.h"
class Scope;
class Object;
class Variable
{
public:
    Variable();
    virtual ~Variable();
    static int getVariableTypeForString(std::string str);
    static Variable getFromPointer(Variable* variable);
    std::string name;
    int type;
    std::string type_name;
    
    // The scope this is a part of. For example if this variable is declared in an IF statement the scope will be the IF statement scope.
    Scope* scope;
    // The object this variable is a part of. NULL if there is no object.
    Object* object;
    
    // Is this a private, protected or public variable?
    MODIFIER_ACCESS access;
    struct Value value;
};
#endif
