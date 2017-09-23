#ifndef NATIVEFUNCTION_H
#define NATIVEFUNCTION_H

#include "function.h"
class NativeFunction : public Function
{
    public:
        NativeFunction(std::string name, std::function<void(std::vector<Value>, Value* return_value, std::shared_ptr<Object> object)> entrypoint);
        virtual ~NativeFunction();
        virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    private:
        std::function<void(std::vector<Value>, Value* return_value, std::shared_ptr<Object> object)> entrypoint;
};

#endif
