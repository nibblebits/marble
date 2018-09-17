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

#ifndef TRYNODE_H
#define TRYNODE_H
#include "inode.h"
#include <memory>
class Validator;
class Interpreter;
class BodyNode;
class VarNode;
class TryNode : public InterpretableNode
{
    public:
        TryNode();
        virtual ~TryNode();
        virtual void test(Validator* validator, struct extras extra);
        virtual Value interpret(Interpreter* interpreter, struct extras extra);
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
        BodyNode* try_body;
        BodyNode* finally_body;
        /** The variable node representing the variable that the exception that was thrown should be stored in */
        VarNode* catch_varnode;
        BodyNode* catch_body;
};
#endif
