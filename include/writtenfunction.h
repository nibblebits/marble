#ifndef WRITTENFUNCTION_H
#define WRITTENFUNCTION_H

#include "singlefunction.h"
class SystemHandler;
class FunctionNode;
class WrittenFunction : public SingleFunction
{
 public:
        WrittenFunction(SystemHandler* handler, FunctionNode* function_node,  std::vector<VarType> argument_types, VarType return_type);
        virtual ~WrittenFunction();
        virtual void invoke_impl(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
 private:
        FunctionNode* fnode;
};
#endif
