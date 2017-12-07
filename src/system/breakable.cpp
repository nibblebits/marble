#include "breakable.h"

Breakable::Breakable()
{
    this->break_type = BREAK_TYPE_NO_BREAK;
}
Breakable::~Breakable()
{

}
void Breakable::breakNow(BREAK_TYPE type)
{
    this->break_type = type;
    this->didBreak(type);
}

bool Breakable::isBroken()
{
    return this->break_type != BREAK_TYPE_NO_BREAK;
}

BREAK_TYPE Breakable::getBreakType()     
{
    return this->break_type;
}
