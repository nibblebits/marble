#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
#include "object.h"
class Function
{
    public:
        Function(std::string name);
        virtual ~Function();
        std::string name;
        std::string getName();
        virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object) = 0;
};
#endif
