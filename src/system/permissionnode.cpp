/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "permissionnode.h"
#include "bodynode.h"
#include "expnode.h"
#include "permissionobject.h"
#include "permissionsobject.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
#include <iostream>
PermissionNode::PermissionNode() : ExpressionInterpretableNode(NODE_TYPE_PERMISSION_NODE)
{

}
PermissionNode::~PermissionNode()
{

}

Value PermissionNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;

    // Ok we need to interpret this permission expression so that we can get the permissions object
    Value exp_result = this->exp_node->interpret(interpreter, extra);
    std::shared_ptr<PermissionsObject> permissions = std::dynamic_pointer_cast<PermissionsObject>(exp_result.ovalue);
    // Now lets create a new scope and set the permissions

    interpreter->new_parented_scope();
    Scope* scope = interpreter->getCurrentScope();
    scope->permissions = permissions;

    try
    {
        // Let's interpret the body now that the scope permissions are set correctly
        body_node->interpret(interpreter, extra);
    }
    catch(...)
    {
        interpreter->finish_parented_scope();
        throw;
    }
    return v;
}
void PermissionNode::test(Validator* validator, struct extras extra)
{
    struct Evaluation evaluation = exp_node->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    if (evaluation.datatype.type != VARIABLE_TYPE_OBJECT)
        throw TestError("Permission scopes must take a valid Permissions object for their expression but a " + evaluation.datatype.value + " was provided");

    Class* c = validator->getClassSystem()->getClassByName(evaluation.datatype.value);
    if (!c->instanceOf("Permissions"))
    {
        throw TestError("Permission scopes must take a valid Permissions object for their expression but the object provided is of type " + evaluation.datatype.value + " and it is not an instance of PermissionsObject");
    }

    // Test the body node
    body_node->test(validator, extra);
}

void PermissionNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of permission nodes is not supported");
}