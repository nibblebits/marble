#ifndef BREAKEXCEPTION_H
#define BREAKEXCEPTION_H

#include "statics.h"
class BreakException 
{
public:
    BreakException(BREAK_TYPE type);
    virtual ~BreakException();
    BREAK_TYPE type;
};
#endif