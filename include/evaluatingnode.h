#ifndef EVALUATINGNODE_H
#define EVALUATINGNODE_H
#include "node.h"
#include "statics.h"
class SystemHandler;
class Variable;
struct Evaluation
{
    EVALUATION_TYPE type = 0;
    struct datatype
    {
        VARIABLE_TYPE type = 0;
        std::string value = "";
    } datatype;
    
    Variable* variable = NULL;
};

class EvaluatingNode : public Node
{
public:
    EvaluatingNode(NODE_TYPE type);
    virtual ~EvaluatingNode();
    struct Evaluation evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation);
    void evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
protected:
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation) { /* Nothing to evaluate if this is not overrided */};
};
#endif
