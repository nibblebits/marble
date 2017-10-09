#include "identifiernode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "object.h"
#include <iostream>

IdentifierNode::IdentifierNode() : ExpressionInterpretableNode(NODE_TYPE_IDENTIFIER)
{
    this->value = "";
}

IdentifierNode::~IdentifierNode()
{

}

void IdentifierNode::test(Validator* validator)
{
    // Let's first see if the variable is declared
    Scope* current_scope = validator->getCurrentScope();
    Variable* variable = current_scope->getVariableAnyScope(this->value);
    if (variable == NULL)
        throw std::logic_error("variable \"" + this->value + "\" is not declared");
    
    if (!validator->isExpecting())
        return;
        
    // We now need to check if the type is valid
    VALUE_TYPE expecting_type = validator->getExpectingType();
    if (variable->type != expecting_type)
        throw std::logic_error("a " + variable->type_name + " was provided");
    
}

Value IdentifierNode::interpret(Interpreter* interpreter)
{
    Variable* var = interpreter->getVariableByName(this->value);
    Value v = var->value;
    return v;
}

void IdentifierNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    // Nothing yet.
}
