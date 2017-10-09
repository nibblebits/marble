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
    return DataType::isPrimitiveDataType(this->value);
}

void KeywordNode::evaluate_impl(SystemHandler* handler, struct Evaluation* evaluation)
{
    if (!isDataTypeKeyword())
        throw std::logic_error("Nothing to evaluate.");
        
    evaluation->type |= EVALUATION_TYPE_DATATYPE;
    evaluation->datatype.type = Variable::getVariableTypeForString(this->value);
    evaluation->datatype.value = this->value;
}
