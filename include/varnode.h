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

#ifndef VARNODE_H
#define VARNODE_H
#include <memory>
#include <string>
#include "node.h"
#include "token.h"
#include "statics.h"
#include "node.h"
#include "evaluatingnode.h"
#include "inode.h"
class ExpressionInterpretableNode;
class VarNode : public InterpretableNode
{
public:
    VarNode();
    virtual ~VarNode();
    virtual void test(Validator* validator, struct extras extra={});
    virtual Value interpret(Interpreter* interpreter, struct extras extra={});

    /**
     * Attempts to interpret and handle operator overloading assignments
     * if this is invalid for this assignment it returns false otherwise true
     */
    bool handleOperatorOverloadIfValid(Interpreter* interpreter, std::string type_str, Variable* variable, ExpressionInterpretableNode* value_node);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    std::string getTypeAsString();
    bool isArray();
    bool isPrimitive();
    bool isObject();
    EvaluatingNode* type;
    std::string name;
    InterpretableNode* value;
    
    // Is this a private, protected or public variable?
    MODIFIER_ACCESS access;
    
    // The array dimensions. Zero if this is not an array
    int dimensions;
    
private:
};

#endif
