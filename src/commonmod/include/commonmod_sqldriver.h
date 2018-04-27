#ifndef COMMONMOD_SQLDRIVER
#define COMMONMOD_SQLDRIVER

#include "object.h"
#include <vector>


/**
 * An abstract pure SQLDriver class. Extend this if you wish to create your own SQL driver
 */

class ModuleSystem;
class Interpreter;
class CommonModule_SqlDriver : public Object
{
public:
    CommonModule_SqlDriver(Class* c);
    virtual ~CommonModule_SqlDriver();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    static void newInterpreter(Interpreter* interpreter);

    // Native SQLDriver functions
    static void SQLDriver_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLDriver_registerDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLDriver_getDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
private:

};

#endif