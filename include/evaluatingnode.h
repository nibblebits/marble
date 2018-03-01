#ifndef EVALUATINGNODE_H
#define EVALUATINGNODE_H
#include "node.h"
#include "statics.h"
#include "vartype.h"
#include "extras.h"
class SystemHandler;
class Variable;
struct Evaluation
{
    EVALUATION_TYPE type = 0;
    VarType datatype;
    Variable* variable = NULL;
    struct extras extra;
};

class EvaluatingNode : public Node
{
public:
    EvaluatingNode(NODE_TYPE type);
    virtual ~EvaluatingNode();
    struct Evaluation evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation);
    void evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
protected:
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation) = 0;
};
#endif
