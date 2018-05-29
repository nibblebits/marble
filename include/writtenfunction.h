#ifndef WRITTENFUNCTION_H
#define WRITTENFUNCTION_H

#include "singlefunction.h"
class SystemHandler;
class FunctionNode;
class WrittenFunction : public SingleFunction
{
 public:
        WrittenFunction(FunctionNode* function_node,  std::vector<VarType> argument_types, VarType return_type);
        virtual ~WrittenFunction();
        virtual void invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        bool hasReturned();        
        FunctionNode* getFunctionNode();
        /** The return node that caused the function to terminate. NULL if none was present.*/
        ReturnNode* return_node;
        /** The value that was returned */
        Value return_value;
        
        /* The function node related to this marble written function*/
        FunctionNode* fnode;
};
#endif
