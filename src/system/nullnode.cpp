#include "nullnode.h"
#include "exceptions/testerror.h"
#include "validator.h"
#include "interpreter.h"

NullNode::NullNode() : ExpressionInterpretableNode(NODE_TYPE_NULL)
{
}

NullNode::~NullNode()
{

}

void NullNode::test(Validator* validator, struct extras extra)
{
    if (!validator->isExpecting())
        return;
    
    // We now need to check if the type is valid
    VARIABLE_TYPE expecting_type = validator->getExpectingVariableType();
    if (expecting_type != VARIABLE_TYPE_OBJECT || validator->isExpectingArray())
        throw TestError("Attempting to use null on non object type");
}

Value NullNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v.type = VALUE_TYPE_OBJECT;
    v.ovalue = NULL;
    return v;   
}

void NullNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    // For evaluations we will just provide a datatype pointing to Object.
    evaluation->datatype.type = VARIABLE_TYPE_OBJECT;
    evaluation->datatype.value = "Object";
}
