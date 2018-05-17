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
        Function(FUNCTION_TYPE type, std::string name);
        virtual ~Function();
        virtual void invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) = 0;
        std::string getName();
        
        /** The Class that this function is a method of. Set to NULL if this is not a method just a function with no class. */
        Class* cls; 
        FUNCTION_TYPE type;
        std::string name;
        bool is_pure;

        // Is this a private, protected or public variable?
        MODIFIER_ACCESS access;


        // An empty static function used for when a native programmer wants to create a function that points to nothing.
        static void Blank(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};
#endif
