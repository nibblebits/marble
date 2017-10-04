#ifndef TESTABLE_H
#define TESTABLE_H
class Validator;
class Testable
{
public:
    virtual void test(Validator* validator) = 0;
};
#endif
