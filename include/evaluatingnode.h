#ifndef EVALUATINGNODE_H
#define EVALUATINGNODE_H
#include "node.h"
#include "statics.h"
class SystemHandler;
struct Evaluation
{
    EVALUATION_TYPE type = 0;
    struct datatype
    {
        VARIABLE_TYPE type = 0;
        std::string value = "";
    } datatype;
};

class EvaluatingNode : public Node
{
public:
    EvaluatingNode(NODE_TYPE type);
    virtual ~EvaluatingNode();
    struct Evaluation evaluate(SystemHandler* handler);
    void evaluate(SystemHandler* handler, struct Evaluation* evaluation);
protected:
    virtual void evaluate_impl(SystemHandler* handler, struct Evaluation* evaluation) { };
};
#endif
