#include "../include/mathobject.h"
#include "modulesystem.h"
#include <math.h>

MathObject::MathObject(Class *c) : Object(c)
{
}
MathObject::~MathObject()
{
}
std::shared_ptr<Object> MathObject::newInstance(Class *c)
{
    return std::make_shared<MathObject>(c);
}

void MathObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Math
     * 
     * Responsible for all math situations. Provides a handful of standard Math methods to assist you in your mathematical operations
     */

    Class *c = moduleSystem->getClassSystem()->registerClass("Math");
    c->setDescriptorObject(std::make_shared<MathObject>(c));
    /**
     * @class Math
     * Returns the sine of an angle of x radians.
     * 
     * function sin(number value) : number
     */
    c->registerFunction("sin", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_sin);

    /**
     * @class Math
     * Returns the cosine of an angle of x radians
     * 
     * function cos(number value) : number
     */
    c->registerFunction("cos", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_cos);

    /**
     * @class Math
     * Returns the absolute number
     * 
     * function abs(number value) : number
     */
    c->registerFunction("abs", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_abs);

    /**
     * @class Math
     * Returns the principal value of the arc cosine of x, expressed in radians.
     * In trigonometrics, arc cosine is the inverse operation of cosine.
     * 
     * function acos(number x) : number
     */
    c->registerFunction("acos", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_acos);

    /**
     * @class Math
     * Compute area hyperbolic cosine
     * Returns the nonnegative area hyperbolic cosine of x.
     * The area hyperbolic cosine is the inverse operation of the hyperbolic cosine.
     * 
     * function acos(number x) : number
     */
    c->registerFunction("acosh", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_acosh);
}


void MathObject::Math_cos(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set((double) cos(values[0].dvalue));
}

void MathObject::Math_sin(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set((double) sin(values[0].dvalue));
}

void MathObject::Math_abs(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set((double) abs(values[0].dvalue));
}

void MathObject::Math_acos(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set((double) acos(values[0].dvalue));
}


void MathObject::Math_acosh(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set((double) acosh(values[0].dvalue));
}




