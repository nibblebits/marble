#ifndef WRITTENFUNCTION_H
#define WRITTENFUNCTION_H

#include "function.h"
class Interpreter;
class FunctionNode;
class WrittenFunction : public Function
{
 public:
        WrittenFunction(Interpreter* interpreter, FunctionNode* function_node);
        virtual ~WrittenFunction();
        virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
 private:
        Interpreter* interpreter;
        FunctionNode* fnode;
};
#endif
