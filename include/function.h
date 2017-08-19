#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
class Function
{
    public:
        Function(std::string name);
        virtual ~Function();
        std::string name;
        std::string getName();
        virtual void invoke(std::vector<Value> values, Value* return_value) = 0;
};
#endif
