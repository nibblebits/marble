#ifndef NATIVEFUNCTION_H
#define NATIVEFUNCTION_H

#include "singlefunction.h"
#include "statics.h"
#include "typedef_func.h"
class NativeFunction : public SingleFunction
{
    public:
        NativeFunction(std::string name, std::vector<VarType> argument_types, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint);
        virtual ~NativeFunction();
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    private:
        NATIVE_FUNCTION_ENTRYPOINT entrypoint;
};

#endif
