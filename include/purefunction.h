#ifndef PUREFUNCTION_H
#define PUREFUNCTION_H

#include "singlefunction.h"
class PureFunction : public SingleFunction
{
 public:
        PureFunction(std::string function_name, std::vector<VarType> argument_types, VarType return_type);
        virtual ~PureFunction();
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
      
};
#endif
