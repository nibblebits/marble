#ifndef COMMONMODULE_LIST
#define COMMONMODULE_LIST
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
/**
 * The List class is an abstract pure class that all classes wishing to provide List functionality should extend
 */
class CommonModule_List : public Object
{
public:
    CommonModule_List(Class* c);
    virtual ~CommonModule_List();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    static void List_Push(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};


#endif