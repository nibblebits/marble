#ifndef MATHOBJECT_H
#define MATHOBJECT_H

#include "object.h"
#include <map>
#include <string>

class ModuleSystem;
class MathObject : public Object
{
  public:
    MathObject(Class *c);
    virtual ~MathObject();
    virtual std::shared_ptr<Object> newInstance(Class *c);

    static void registerClass(ModuleSystem *moduleSystem);

    // Native Math functions/methods
    static void Math_cos(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
    static void Math_sin(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
    static void Math_abs(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
    static void Math_acos(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
    static void Math_acosh(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
};
#endif