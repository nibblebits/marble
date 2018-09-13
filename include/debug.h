#ifndef DEBUG_H
#define DEBUG_H
#include "config.h"

#ifdef DEBUG_ENABLED
#include <memory>
#include "node.h"
class Scope;
class PermissionsObject;
class Debug
{
public:
    Debug();
    virtual ~Debug();
    static void OutputTabbing(int amount);
    static void PrintValueForNode(Node* value_node, int tabbing=0);
    static void PrintVariablesForScope(Scope* scope);
    static void PrintPermissions(std::shared_ptr<PermissionsObject> permissions);
};
#endif

#endif
