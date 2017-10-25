#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
#include "object.h"
#include "statics.h"

class Class;
class Function
{
    public:
        Function(FUNCTION_TYPE type, std::string name);
        virtual ~Function();
        std::string getName();
        virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object) = 0;
        /** The Class that this function is a method of. Set to NULL if this is not a method just a function with no class. */
        Class* cls; 
        FUNCTION_TYPE type;
        std::string name;
};
#endif
