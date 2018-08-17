#ifndef MATHMODULE_H
#define MATHMODULE_H
#include "module.h"
class Scope;
class MathModule : public Module
{
public:
    MathModule();
    virtual ~MathModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:
    // Native Math functions/methods
    static void Math_cos(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Math_sin(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Math_abs(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Math_acos(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Math_acosh(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif