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

#include "dowhilenode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
#include "exceptions/breakexception.h"
DoWhileNode::DoWhileNode() : InterpretableNode(NODE_TYPE_DO_WHILE)
{
}

DoWhileNode::~DoWhileNode()
{
}

void DoWhileNode::test(Validator *validator, struct extras extra)
{
    try
    {
        this->exp->test(validator);
        this->body->test(validator);
    }
    catch (TestError &ex)
    {
        throw TestError(ex.getMessage() + " for while statement");
    }
}

Value DoWhileNode::interpret(Interpreter *interpreter, struct extras extra)
{
    Value v;
    try
    {
        do
        {
            try
            {
                body->interpret(interpreter);
                v = exp->interpret(interpreter);
            }
            catch (BreakException &ex)
            {
                v = exp->interpret(interpreter);
                if (ex.type == BREAK_TYPE_BREAK)
                    break;
                if (ex.type == BREAK_TYPE_CONTINUE)
                    continue;
            }
        } while (v.dvalue == 1);
    }
    catch (...)
    {
        throw;
    }

    return v;
}

void DoWhileNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    throw std::logic_error("Evaluation of do while nodes is not supported");
}
