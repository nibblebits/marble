#ifndef DEBUG_H
#define DEBUG_H
#include "config.h"

#ifdef DEBUG_ENABLED
#include <memory>
#include "node.h"
class Debug
{
public:
    Debug();
    virtual ~Debug();
    static void OutputTabbing(int amount);
    static void PrintValueForNode(Node* value_node, int tabbing=0);
};
#endif

#endif
