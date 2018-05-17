#include "keywordnode.h"
#include "statics.h"
#include "datatype.h"
#include "variable.h"

KeywordNode::KeywordNode() : EvaluatingNode(NODE_TYPE_KEYWORD)
{
    this->value = "";
}

KeywordNode::~KeywordNode()
{

}

bool KeywordNode::isDataTypeKeyword()
{
    // Keywords can only represent primitive datatypes.
    return DataType::isPrimitiveDataType(this->value);
}

void KeywordNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    // The keyword node only evaluates data types.
    if (!(expected_evaluation & EVALUATION_TYPE_DATATYPE))
        return;
    
    if (!isDataTypeKeyword())
        throw std::logic_error("Not a datatype keyword.");
        
    evaluation->type |= EVALUATION_TYPE_DATATYPE;
    evaluation->datatype.type = Variable::getVariableTypeForString(this->value);
    evaluation->datatype.value = this->value;
}
