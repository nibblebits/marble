#ifndef EXTRAS_H
#define EXTRAS_H

class Scope;
struct extras
{
    // The scope that is accessing a function or variable
    Scope* accessors_scope=NULL;
};

#endif