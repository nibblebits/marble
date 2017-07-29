#include "varnode.h"

Varnode::Varnode() : Node(NODE_TYPE_VARIABLE_DECLARATION)
{
	this->var_type = NULL;	
	this->var_name = NULL;
}

Varnode::~Varnode()
{

}
