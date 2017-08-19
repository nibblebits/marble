#ifndef INTERPRETABLE_H
#define INTERPRETABLE_H

#include "statics.h"
#include "value.h"
class Interpretable
{
public:
    virtual Value interpret() = 0;
};

#endif
