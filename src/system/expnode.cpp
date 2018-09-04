#include "expnode.h"
#include "variable.h"
#include "identifiernode.h"
#include "literalnode.h"
#include "statics.h"
#include "object.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptionobject.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include "exceptions/evaluationexception.h"
#include "debug.h"
#include "object.h"
#include "operator.h"
#include "function.h"
#include "class.h"
#include <unistd.h>
#include <iostream>
#include <memory>
ExpNode::ExpNode() : ExpressionInterpretableNode(NODE_TYPE_EXPRESSION)
{
    this->left = NULL;
    this->right = NULL;
    this->op = "";
}

ExpNode::~ExpNode()
{
}

bool ExpNode::isAssignmentOperator()
{
    return this->op == "=" || this->op == "+=" || this->op == "-=" || this->op == "*=" || this->op == "/=";
}

Value ExpNode::objectCompareOperatorOverloadExecute(Value &value1, Value &value2, std::string op, Interpreter *interpreter, bool dont_flip)
{
    Value result;
    // Value1 and 2 differ but value1 is an object so let's see if we can pull a method to get the value we are looking for
    if (value1.ovalue != NULL)
    {
        Class *c = value1.ovalue->getClass();
        Function *function = c->getFunctionByName("operator:" + op);
        if (function == NULL && !dont_flip)
        {
            result = objectCompareOperatorOverloadExecute(value2, value1, op, interpreter, true);
        }
        else
        {
            value1.ovalue->runThis([&] {
                function->invoke(interpreter, {value1, value2}, &result, value1.ovalue, interpreter->getCurrentScope());
            },
                                   interpreter, c);
        }
    }
    else if (!dont_flip)
    {
        result = objectCompareOperatorOverloadExecute(value2, value1, op, interpreter, true);
    }

    return result;
}

Value ExpNode::compareGetResult(Value &value1, Value &value2, std::string op, Interpreter *interpreter)
{
    Value result;
    if ((value1.type == VALUE_TYPE_OBJECT && value2.type != VALUE_TYPE_OBJECT) || (value2.type == VALUE_TYPE_OBJECT && value1.type != VALUE_TYPE_OBJECT))
    {
        result = objectCompareOperatorOverloadExecute(value1, value2, op, interpreter);
    }

    // No compare operator is overloaded let's do the compare on the values
    if (result.type == -1)
    {
        if (op == "==")
        {
            result = (value1 == value2);
        }
        else if (op == "!=")
        {
            result = (value1 != value2);
        }
    }

    return result;
}

Value ExpNode::mathify(Value &value1, Value &value2, std::string op, Interpreter *interpreter)
{
    Value result;
    if (op == "+" || op == "+=")
    {
        if (value1.type == VALUE_TYPE_OBJECT && value2.type != VALUE_TYPE_OBJECT)
        {
            // Value1 and 2 differ but value1 is an object so let's see if we can pull a method to get the value we are looking for
            if (value1.ovalue != NULL)
            {
                if (value2.type == VALUE_TYPE_NUMBER)
                {
                    Class *c = value1.ovalue->getClass();
                    Function *toNumber = c->getFunctionByNameAndArguments("toNumber", {});
                    value1.ovalue->runThis([&] {
                        toNumber->invoke(interpreter, {}, &value1, value1.ovalue, interpreter->getCurrentScope());
                    },
                                           interpreter, c);
                }
                else if (value2.type == VALUE_TYPE_STRING)
                {
                    Class *c = value1.ovalue->getClass();
                    Function *toString = c->getFunctionByNameAndArguments("toString", {});
                    value1.ovalue->runThis([&] {
                        toString->invoke(interpreter, {}, &value1, value1.ovalue, interpreter->getCurrentScope());
                    },
                                           interpreter, c);
                }
            }
        }
        result = value1 + value2;
    }
    else if (op == "-" || op == "-=")
    {
        result = value1 - value2;
    }
    else if (op == "*" || op == "*=")
    {
        result = value1 * value2;
    }
    else if (op == "/" || op == "/=")
    {
        result = value1 / value2;
    }
    else if (op == ">")
    {
        result = value1 > value2;
    }
    else if (op == ">=")
    {
        result = value1 >= value2;
    }
    else if (op == "<=")
    {
        result == value1 <= value2;
    }
    else if (op == "<")
    {
        result = value1 < value2;
    }
    else if (op == "==")
    {
        result = compareGetResult(value1, value2, "==", interpreter);
    }
    else if (op == "!=")
    {
        result = compareGetResult(value1, value2, "!=", interpreter);
    }
    else if (op == "&&")
    {
        result = value1 && value2;
    }
    else if (op == "||")
    {
        result = value1 || value2;
    }
    else if (op == "=")
    {
        result = value2;
    }
    else
    {
        throw std::logic_error("Value ExpNode::mathify(Value value1, Value2 value2, std::string op, Interperter* interpreter): Invalid operator provided");
    }

    return result;
}

/*
* this must be split into multiple methods in the same way the test routine is.
*/
Value ExpNode::interpret(Interpreter *interpreter, struct extras extra)
{
    Value result;
    if (isAssignmentOperator())
    {
        // This is to be an assignment. We must interpret the left node and then we will have the variable we need to assign to the right node
        Value left_v = left->interpret(interpreter);
        Variable *var_to_assign = left_v.holder;
        if (var_to_assign == NULL)
            throw std::logic_error("Problem as the value returned when interpreting the left node of this assignment does not have a holder: Value string: " + left_v.svalue);

        // Ok now lets get the value and assign it to the variable
        Value right_v = right->interpret(interpreter);

        if (this->op == "=")
        {
            result = right_v;
        }
        else
        {
            result = mathify(left_v, right_v, this->op, interpreter);
        }

        if (var_to_assign->is_locked)
        {
            // The variable we want to assign is locked so we just have to throw an exception and refuse this
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))), "The variable " + var_to_assign->name + " is locked and cannot be modified");
        }
        var_to_assign->setValue(result, interpreter);
        return result;
    }

    Value left_v = this->left->interpret(interpreter);

    /* Logical operators have rules for when we should continue or not */
    if (this->op == "&&" || this->op == "||")
    {
        if (this->op == "||")
        {
            // If our left expression is true then we can just return left_v here as the expression is true.
            if (left_v.dvalue)
                return left_v;
        }
        else
        {
            // If the left expression is false then we should not continue to the next expression as this is an AND operator
            if (!left_v.dvalue)
                return left_v;
        }
    }
    if (this->op == ".")
    {
        // Left_v has the object to access
        std::shared_ptr<Object> obj = left_v.ovalue;
        if (obj == NULL)
        {
            // Throw a NULL pointer exception here. This will be a SystemException so it may be caught from within marble.
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("NullPointerException"), {})));
        }

        Class *c = NULL;
        if (left_v.holder != NULL)
        {
            // If the left variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (left_v.holder->name == "super")
                c = obj->getClass()->parent;
        }

        Scope *accessors_scope = (extra.accessors_scope != NULL ? extra.accessors_scope : interpreter->getCurrentScope());
        // Interpret the right node on the object scope and return the result.
        obj->runThis([&] {
            struct extras extra;
            extra.accessors_scope = accessors_scope;
            extra.is_object_exp = true;
            extra.current_object = obj;
            result = this->right->interpret(interpreter, extra);
        },
                     interpreter, c);
        return result;
    }

    Value right_v = this->right->interpret(interpreter);
    result = mathify(left_v, right_v, this->op, interpreter);
    return result;
}

void ExpNode::test(Validator *validator, struct extras extra)
{
    if (this->op == ".")
    {
        test_obj_access(validator, extra);
    }
    else if (this->isAssignmentOperator())
    {
        test_assign(validator);
    }
    else if (this->op == "&&" || this->op == "||")
    {
        test_logical_operator_exp(validator);
    }
    else if (Operator::isCompareOperator(this->op))
    {
        test_compare_expression(validator);
    }
    else
    {
        test_regular_exp(validator);
    }
}

void ExpNode::test_obj_access(Validator *validator, struct extras extra)
{
    /* While testing the left node we don't really want to care about testing weather or not a type matches
     * as we only care about this for the right operand. For example if an assignment is expecting a string it does not make sense
     * to check for the string on the left operand. "obj.var" The left operand is "obj" it would basically be saying I expect the object to be a string for the expression: 
     * "string str = obj.var;" This clearly does not make sense what we really want is to expect that the variable "var" of the object "obj" is a string.*/

    // Save the validators state and give us a blank state with no requirements. See comment above.
    validator->save();
    left->test(validator, extra);
    validator->restore();

    struct Evaluation evaluation;
    evaluation.extra = extra;
    Scope *accessors_scope = (extra.accessors_scope != NULL ? extra.accessors_scope : validator->getCurrentScope());
    evaluation.extra.accessors_scope = accessors_scope;
    left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_TYPE_VARIABLE | EVALUATION_FROM_VARIABLE, &evaluation);

    Class *c = NULL;
    /* If the datatype is an array but we are also not accessing an array index we should switch the datatype to array 
       which will give access to the array class.
       abc[0] = Array but the left node is NODE_TYPE_ARRAY as we are accessing array indexes. Here we use the datatype of abc
       abc.size() = Left is an Array but we are not accessing it as an array so use the array class*/
    std::string datatype_name = (evaluation.datatype.isArray() && left->type != NODE_TYPE_ARRAY) ? "array" : evaluation.datatype.value;
    if (!validator->isClassIgnored(datatype_name))
    {
        std::shared_ptr<Object> obj = validator->getClassObject(datatype_name);
        if (obj == NULL)
            throw TestError("This is not an object but a " + datatype_name);

        c = obj->getClass();
        if (evaluation.variable != NULL)
        {
            // If the evaluated variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (evaluation.variable->name == "super")
                c = obj->getClass()->parent;
        }

        obj->runThis([&] {
            struct extras extra;
            extra.accessors_scope = accessors_scope;
            extra.is_object_exp = true;
            extra.current_object = obj;
            this->right->test(validator, extra);
        },
                     validator, c);
    }
}

void ExpNode::test_assign(Validator *validator)
{
    left->test(validator);
    struct Evaluation left_evaluation = left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    std::string left_type_str = left_evaluation.datatype.value;
    bool ignore_expecting = false;
    if (Variable::getVariableTypeForString(left_type_str) == VARIABLE_TYPE_OBJECT)
    {
        /* Let's just check if this class has overridden the "=" operator 
            * if it does we should not instruct the validator to expects a given type
            */
        Class *c = validator->getClassSystem()->getClassByName(left_type_str);

        struct Evaluation right_evaluation = this->right->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
        if (c->hasOverloadedOperator("=", right_evaluation.datatype.value))
        {
            ignore_expecting = true;
        }
    }

    if (!ignore_expecting)
        validator->expecting(left_type_str);
    try
    {
        right->test(validator);
    }
    catch (TestError &ex)
    {
        throw TestError(std::string(ex.what()) + " but this assignment requires a " + left_type_str + (left_evaluation.datatype.isArray() ? " with " + std::to_string(left_evaluation.datatype.dimensions) + " array dimensions" : ""));
    }

    if (!ignore_expecting)
        validator->endExpecting();
}

/**
 * This method is no longer used due to how difficult it currently is to implement operator overloading other than "="
 * I will return and get the rest working eventually
 */
bool ExpNode::checkOperatorOverloadPossible(Validator *validator, ExpressionInterpretableNode *left_node, ExpressionInterpretableNode *right_node, bool no_loop)
{
    bool ignore_expecting = false;
    struct Evaluation left_evaluation = left_node->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    std::string left_type_str = left_evaluation.datatype.value;
    if (Variable::getVariableTypeForString(left_type_str) == VARIABLE_TYPE_OBJECT)
    {
        /* Let's just check if this class has overridden the operator provided
            * if it does we should not instruct the validator to expects a given type
            */
        Class *c = validator->getClassSystem()->getClassByName(left_type_str);
        struct Evaluation right_evaluation = right_node->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
        if (c->hasOverloadedOperator(this->op, left_evaluation.datatype.value, right_evaluation.datatype.value))
        {
            ignore_expecting = true;
        }
    }

    if (!ignore_expecting && !no_loop)
    {
        // As we should not ignore expecting for the left_node with the right_node let's check the right_node with the left_node
        return checkOperatorOverloadPossible(validator, right_node, left_node, true);
    }

    return ignore_expecting;
}

void ExpNode::test_logical_operator_exp(Validator *validator)
{
    validator->save();
    validator->expecting("boolean");
    try
    {
        this->left->test(validator);
        this->right->test(validator);
    }
    catch (TestError &ex)
    {
        throw TestError("expecting a boolean but " + ex.getMessage());
    }
    validator->endExpecting();
    validator->restore();
}

void ExpNode::test_compare_expression(Validator *validator)
{
    /*
         * We are a compare operator so let's ensure that the validator is expecting a boolean
         */
    if (validator->isExpecting() && validator->getExpectingType() != "boolean")
    {
        throw TestError("a boolean was provided");
    }

    // Save the validator as the left node does not have to be a boolean
    validator->save();
    left->test(validator);

    struct Evaluation left_evaluation = left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    std::string left_type_str = left_evaluation.datatype.value;
    validator->expecting(left_type_str);
    try
    {
        right->test(validator);
    }
    catch (TestError &ex)
    {
        throw TestError("The expression requires the right to be the same type of the left a type of \"" + left_type_str + "\"");
    }
    validator->endExpecting();
    validator->restore();
}

void ExpNode::test_regular_exp(Validator *validator)
{

    // Test the left and right nodes
    left->test(validator);

    validator->save();

    /**
     * There is a chance that operator overloading has taken place in which case those methods should be invoked for this expression
     */
    struct Evaluation left_evaluation = left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    std::string left_type_str = left_evaluation.datatype.value;

    validator->expecting(left_type_str);

    try
    {
        right->test(validator);
    }
    catch (TestError &ex)
    {
        throw TestError(std::string(ex.what()) + " but this expression requires a " + left_type_str + (left_evaluation.datatype.isArray() ? " with " + std::to_string(left_evaluation.datatype.dimensions) + " array dimensions" : ""));
    }

    validator->endExpecting();

    validator->restore();
}

void ExpNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    // If we have no accessors scope lets set one
    if (evaluation->extra.accessors_scope == NULL)
        evaluation->extra.accessors_scope = handler->getCurrentScope();

    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        VarType left_vartype;
        left->evaluate(handler, expected_evaluation, evaluation);
        left_vartype = evaluation->datatype;
        FunctionSystem *old_fc_system;
        Scope *old_scope;
        if (this->op == ".")
        {
            old_fc_system = handler->getFunctionSystem();
            old_scope = handler->getCurrentScope();

            std::string class_obj_name = evaluation->datatype.value;
            /* If the datatype is an array but we are also not accessing an array index we should switch the datatype to array 
       which will give access to the array class.
       abc[0] = Array but the left node is NODE_TYPE_ARRAY as we are accessing array indexes. Here we use the datatype of abc
       abc.size() = Left is an Array but we are not accessing it as an array so use the array class*/
            if (evaluation->datatype.dimensions > 0 && left->type != NODE_TYPE_ARRAY)
            {
                class_obj_name = "array";
            }
            Validator *validator = NULL;
            if (handler->getType() == SYSTEM_HANDLER_VALIDATOR)
            {
                validator = (Validator *)handler;
            }
            else if (handler->getType() == SYSTEM_HANDLER_INTERPRETER)
            {
                validator = ((Interpreter *)handler)->getValidator();
            }
            else
            {
                throw std::logic_error("Invalid SystemHandler provided that cannot be used to get class object");
            }

            std::shared_ptr<Object> obj = validator->getClassObject(class_obj_name);
            handler->setCurrentScope(obj.get());
            handler->setFunctionSystem(obj->getClass());
            evaluation->extra.is_object_exp = true;
        }

        right->evaluate(handler, expected_evaluation, evaluation);
        if (this->op != ".")
        {
            if (left_vartype != evaluation->datatype)
            {
                throw EvaluationException("Evaluation error as both expression nodes differ in types", left_vartype.value, evaluation->datatype.value);
            }
        }

        if (this->op == ".")
        {
            handler->setCurrentScope(old_scope);
            handler->setFunctionSystem(old_fc_system);
        }
    }
}
