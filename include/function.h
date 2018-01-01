#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
#include "object.h"
#include "statics.h"
#include "typedef_func.h"

class ReturnNode;
class SystemHandler;
class Class;
class Function
{
    public:
        Function(SystemHandler* handler, FUNCTION_TYPE type, std::string name);
        virtual ~Function();
        virtual void invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object) = 0;
        std::string getName();
        SystemHandler* getSystemHandler();
        
        /** The Class that this function is a method of. Set to NULL if this is not a method just a function with no class. */
        Class* cls; 
        FUNCTION_TYPE type;
        std::string name;
	    SystemHandler* sys_handler;
};
#endif
