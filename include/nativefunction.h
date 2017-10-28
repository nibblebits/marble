#ifndef NATIVEFUNCTION_H
#define NATIVEFUNCTION_H

#include "singlefunction.h"
class NativeFunction : public SingleFunction
{
    public:
        NativeFunction(std::string name, std::vector<VarType> argument_types, VarType return_type, std::function<void(std::vector<Value>, Value* return_value, std::shared_ptr<Object> object)> entrypoint);
        virtual ~NativeFunction();
        virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    private:
        std::function<void(std::vector<Value>, Value* return_value, std::shared_ptr<Object> object)> entrypoint;
};

#endif
