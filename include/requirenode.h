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

#ifndef REQUIRENODE_H
#define REQUIRENODE_H
#include "inode.h"
#include "splitter.h"
#include <string>
#include <deque>
class ExpressionInterpretableNode;
class RequireNode : public InterpretableNode
{
public:
    RequireNode();
    virtual ~RequireNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);

    std::string filename;
    // The AST of the required marble source file once it has been tested
    InterpretableNode* required_ast;
private:
    Splitter splitter;
    std::string code_result;
};
#endif
