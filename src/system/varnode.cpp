#include "varnode.h"

Varnode::Varnode() : Node(NODE_TYPE_VARIABLE_DECLARATION)
{
	this->type = NULL;	
	this->name = NULL;
	this->value = NULL;
}

Varnode::~Varnode()
{

}
