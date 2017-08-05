#ifndef DEBUG_H
#define DEBUG_H
#include "config.h"

#ifdef DEBUG_ENABLED
#include <memory>
#include "node.h"
class Debug {
public:
	Debug();
	virtual ~Debug();
	static void PrintValueForNode(std::shared_ptr<Node> value_node);
};
#endif

#endif
