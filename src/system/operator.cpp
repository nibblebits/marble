#include "operator.h"
bool Operator::isCompareOperator(std::string op)
{
    return op == "==" || op == "!=" || op == ">" || op == "<" || op == "<=" || op == ">=";
}