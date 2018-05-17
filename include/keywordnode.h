#ifndef KEYWORDNODE_H
#define KEYWORDNODE_H
#include "evaluatingnode.h"
#include <string>
class KeywordNode : public EvaluatingNode
{
public:
    KeywordNode();
    virtual ~KeywordNode();
    std::string value;
    bool isDataTypeKeyword();
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};

#endif
