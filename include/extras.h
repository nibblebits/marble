#ifndef EXTRAS_H
#define EXTRAS_H
#include <memory>
#include "statics.h"
class Scope;
class Object;
struct extras
{
    // The scope that is accessing a function or variable
    Scope* accessors_scope=NULL;

    /* This is set to true if we are currently doing an object expression such as a.b.c.
     * It is true only for the expression and not for the actual function call Seek isAccessingObject() in the SystemHandler if you want that functionality*/
    bool is_object_exp=false;

    /* A bit mask that represents generic states that can apply to different nodes
     * For example when testing a function node you can use EXTRA_STATE_FUNCTION_DECLARATION_ONLY to only test the function declaration
     * and not the body its self
     */
    EXTRA_STATE state = 0;

    // The current object that is being accessed
    std::shared_ptr<Object> current_object=NULL;

    /**
     * This is used to represent the current array index when parsing array indexes.
     * It starts at one. Zero if no array index is present. It is passed only validation of array indexes and it is not passed to the index node.
     */
    int current_array_index = 0;
};

#endif