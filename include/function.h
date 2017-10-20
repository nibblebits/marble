#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
#include "object.h"
#include "statics.h"

class Function
{
    public:
        Function(FUNCTION_TYPE type, std::string name);
        virtual ~Function();
        std::string getName();
        virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object) = 0;
        
        FUNCTION_TYPE type;
        std::string name;
};
#endif
