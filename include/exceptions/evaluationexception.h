#ifndef EVALUATIONEXCEPTION_H
#define EVALUATIONEXCEPTION_H

#include "exception.h"
class EvaluationException : public Exception
{
public:
    EvaluationException(std::string message) : Exception(message) {}
};


#endif