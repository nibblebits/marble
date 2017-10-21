#ifndef WRITTENFUNCTION_H
#define WRITTENFUNCTION_H

#include "singlefunction.h"
class SystemHandler;
class FunctionNode;
class WrittenFunction : public SingleFunction
{
 public:
        WrittenFunction(SystemHandler* sys_handler, FunctionNode* function_node,  std::vector<VarType> argument_types);
        virtual ~WrittenFunction();
        virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
 private:
        SystemHandler* sys_handler;
        FunctionNode* fnode;
};
#endif
