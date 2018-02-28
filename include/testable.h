#ifndef TESTABLE_H
#define TESTABLE_H
#include "extras.h"

class Validator;
class Testable
{
public:
    virtual void test(Validator* validator, struct extras extra={}) = 0;
};
#endif
