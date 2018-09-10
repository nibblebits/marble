#include "../include/mathobject.h"
#include "modulesystem.h"
#include <math.h>
#include <tgmath.h>

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

    /**
     * @class Math
     * 
     * Returns the tangent of an angle of x radians.
     * function tan(number x) : number
     */
    c->registerFunction("tan", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_tan);

    /**
     * @class Math
     * 
     * Returns the principal value of the arc sine of x, expressed in radians.
     * function asin(number x) : number
     */
    c->registerFunction("asin", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_asin);

    /**
     * @class Math
     * 
     * Returns the principal value of the arc tangent of x, expressed in radians.
     * In trigonometrics, arc tangent is the inverse operation of tangent.
     * function atan(number x) : number
     */
    c->registerFunction("atan", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_atan);

    /**
     * @class Math
     * 
        Returns the principal value of the arc tangent of y/x, expressed in radians.
        To compute the value, the function takes into account the sign of both arguments in order to determine the quadrant.
        In C++, this function is overloaded in <valarray> (see valarray atan2).
     * function atan2(number y, number x) : number
     */
    c->registerFunction("atan2", {VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_atan2);

    /**
     * @class Math
     * 
     * Returns the hyperbolic cosine of x.
     * function cosh(number x) : number
     */
    c->registerFunction("cosh", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_cosh);

    /**
     * @class Math
     * 
     * Returns the hyperbolic sine of x.
     * function sinh(number x) : number
     */
    c->registerFunction("sinh", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_sinh);

    /**
     * @class Math
     * 
     * Returns the hyperbolic tangent of x.
     * function tanh(number x) : number
     */
    c->registerFunction("tanh", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_tanh);

    /**
     * @class Math
     * 
     * Returns the area hyperbolic tangent of x.
     * function asinh(number x) : number
     */
    c->registerFunction("asinh", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_asinh);

    /**
     * @class Math
     * 
     * Returns the area hyperbolic sine of x.
     * function asinh(number x) : number
     */
    c->registerFunction("atanh", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_atanh);

    /**
     * @class Math
     * Returns the base-e exponential function of x, which is e raised to the power x: ex.
     * function exp(number x) : number
     */
    c->registerFunction("exp", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_exp);

    /**
     * @class Math
     * 
     * Returns the result of multiplying x (the significand) by 2 raised to the power of exp (the exponent).
     * function ldexp(number x, number exp) : number
     */
    c->registerFunction("ldexp", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_ldexp);

    /**
     * @class Math
     *  Returns the natural logarithm of x.
     * function log(number x) : number
     */
    c->registerFunction("log", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_log);

    /**
     * @class Math
     * Returns the common (base-10) logarithm of x.
     * function log10(number x) : number
     */
    c->registerFunction("log10", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_log10);

    /**
     * @class Math
     *  Returns the base-2 exponential function of x, which is 2 raised to the power x: 2x.
     * function exp2(number x) : number
     */
    c->registerFunction("exp2", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_exp2);

    /**
     * @class Math
     *  Returns the integral part of the logarithm of |x|, using FLT_RADIX as base for the logarithm.
     * function ilogb(number x) : number
     */
    c->registerFunction("ilogb", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_ilogb);

    /**
     * @class Math
     * Returns the natural logarithm of one plus x.
     * function log1p(number x) : number
     */
    c->registerFunction("log1p", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_log1p);

    /**
     * @class Math
     * Returns the binary (base-2) logarithm of x.
     * function log2(number x) : number
     */
    c->registerFunction("log2", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_log2);

    /**
     * @class Math
     *  Returns the logarithm of |x|, using FLT_RADIX as base for the logarithm.
     * function logb(number x) : number
     */
    c->registerFunction("logb", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_logb);

    /**
     * @class Math
     * 
     * Scales x by FLT_RADIX raised to the power of n, returning the same as:
     * scalbn(x,n) = x * FLT_RADIXn
     * Presumably, x and n are the components of a floating-point number in the system; In such a case, this function may be optimized to be more efficient than the theoretical operations to compute the value explicitly.
     * function scalbn(number x, number n) : number
     */
    c->registerFunction("scalbn", {VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_scalbn);

    /**
     * @class Math
     * 
     *  Scales x by FLT_RADIX raised to the power of n, returning the result of computing:
     *  scalbn(x,n) = x * FLT_RADIXn
     * 
     *  Presumably, x and n are the components of a floating-point number in the system; In such a case, this function may be optimized to be more efficient than the theoretical operations to compute the value explicitly.
     * function scalbln(number x, number n) : number
     */
    c->registerFunction("scalbln", {VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_scalbln);

    /**
     * @class Math
     * Returns base raised to the power exponent:
     * function pow(number base, number exponent) : number
     */
    c->registerFunction("pow", {VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_pow);

    /**
     * @class Math
     *  Returns the square root of x.
     * function sqrt(number x) : number
     */
    c->registerFunction("sqrt", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_sqrt);

    /**
     * @class Math
     *  Returns the cubic root of x.
     * function cbrt(number x) : number
     */
    c->registerFunction("cbrt", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_cbrt);

    /**
     * @class Math
     * Returns the hypotenuse of a right-angled triangle whose legs are x and y.
     * The function returns what would be the square root of the sum of the squares of x and y (as per the Pythagorean theorem), but without incurring in undue overflow or underflow of intermediate values.
     * function hypot(number x, number y) : number
     */
    c->registerFunction("hypot", {VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_hypot);

    /**
     * @class Math
     *  Rounds x upward, returning the smallest integral value that is not less than x.
     * function ceil(number x) : number
     */
    c->registerFunction("ceil", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_ceil);

    /**
     * @class Math
     * Rounds x downward, returning the largest integral value that is not greater than x.
     * function floor(number x) : number
     */
    c->registerFunction("floor", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_floor);

    /**
     * @class Math
     * Returns the integral value that is nearest to x, with halfway cases rounded away from zero.
     * function round(number x) : number
     */
    c->registerFunction("round", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_round);


    /**
     * @class Math
     *  Returns the absolute value of x: |x|.
     * function abs(number x) : number
     */
    c->registerFunction("abs", {VarType::fromString("number")}, VarType::fromString("number"), MathObject::Math_abs);

}

void MathObject::Math_cos(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)cos(values[0].dvalue));
}

void MathObject::Math_sin(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)sin(values[0].dvalue));
}

void MathObject::Math_abs(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)abs(values[0].dvalue));
}

void MathObject::Math_acos(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)acos(values[0].dvalue));
}

void MathObject::Math_acosh(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)acosh(values[0].dvalue));
}

void MathObject::Math_tan(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)tan(values[0].dvalue));
}

void MathObject::Math_asin(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)asin(values[0].dvalue));
}

void MathObject::Math_atan(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)atan(values[0].dvalue));
}

void MathObject::Math_atan2(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)atan2(values[0].dvalue, values[1].dvalue));
}

void MathObject::Math_cosh(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)cosh(values[0].dvalue));
}

void MathObject::Math_sinh(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)sinh(values[0].dvalue));
}

void MathObject::Math_tanh(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)tanh(values[0].dvalue));
}

void MathObject::Math_asinh(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)asinh(values[0].dvalue));
}

void MathObject::Math_atanh(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)atanh(values[0].dvalue));
}

void MathObject::Math_exp(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)exp(values[0].dvalue));
}

void MathObject::Math_ldexp(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)ldexp(values[0].dvalue, values[1].dvalue));
}

void MathObject::Math_log(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)log(values[0].dvalue));
}

void MathObject::Math_log10(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)log10(values[0].dvalue));
}

void MathObject::Math_exp2(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)exp2(values[0].dvalue));
}

void MathObject::Math_ilogb(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)ilogb(values[0].dvalue));
}

void MathObject::Math_log1p(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)log1p(values[0].dvalue));
}

void MathObject::Math_log2(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)log2(values[0].dvalue));
}

void MathObject::Math_logb(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)logb(values[0].dvalue));
}

void MathObject::Math_scalbn(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)scalbn(values[0].dvalue, (int)values[1].dvalue));
}

void MathObject::Math_scalbln(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)scalbln(values[0].dvalue, (int)values[1].dvalue));
}

void MathObject::Math_pow(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)pow(values[0].dvalue, values[1].dvalue));
}

void MathObject::Math_sqrt(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)sqrt(values[0].dvalue));
}

void MathObject::Math_cbrt(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)cbrt(values[0].dvalue));
}

void MathObject::Math_hypot(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)hypot(values[0].dvalue, values[1].dvalue));
}

void MathObject::Math_ceil(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)ceil(values[0].dvalue));
}

void MathObject::Math_floor(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)floor(values[0].dvalue));
}

void MathObject::Math_round(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set((double)round(values[0].dvalue));
}