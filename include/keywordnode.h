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
    virtual void evaluate_impl(SystemHandler* handler, struct Evaluation* evaluation);
};

#endif
