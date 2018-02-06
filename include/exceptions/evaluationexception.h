#ifndef EVALUATIONEXCEPTION_H
#define EVALUATIONEXCEPTION_H

#include "exception.h"
#include <string>
class EvaluationException : public Exception
{
public:
    EvaluationException(std::string message, std::string type1, std::string type2) : Exception(message) { this->type1 = type1; this->type2 = type2;}
    std::string type1;
    std::string type2;
};


#endif