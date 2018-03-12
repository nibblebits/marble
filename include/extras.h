#ifndef EXTRAS_H
#define EXTRAS_H

class Scope;
struct extras
{
    // The scope that is accessing a function or variable
    Scope* accessors_scope=NULL;
    /* This is set to true if we are currently doing an object expression such as a.b.c.
     * It is true only for the expression and not for the actual function call Seek isAccessingObject() in the SystemHandler if you want that functionality*/
    bool is_object_exp=false;
};

#endif