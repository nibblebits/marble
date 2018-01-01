#ifndef IOMODULE_H
#define IOMODULE_H
#include "module.h"
class IOModule : public Module
{
public:
    IOModule();
    virtual ~IOModule();
    void Init();

private:
    // Native IO functions/methods
    void print(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
};
#endif