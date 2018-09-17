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

#include "fornode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
#include "exceptions/breakexception.h"
#include "listnode.h"
ForNode::ForNode() : InterpretableNode(NODE_TYPE_FOR)
{
}

ForNode::~ForNode()
{
}

void ForNode::test(Validator *validator, struct extras extra)
{
    // Let's create a new parented scope for this test
    validator->new_parented_scope();
    try
    {
        this->init->test(validator);
        this->cond->test(validator);
        this->loop->test(validator);
        this->body->test(validator);
    }
    catch (TestError &ex)
    {
        validator->finish_parented_scope();
        throw TestError(ex.getMessage() + " for \"for\" loop");
    }
    validator->finish_parented_scope();
}

Value ForNode::interpret(Interpreter *interpreter, struct extras extra)
{
    interpreter->new_parented_scope();
    init->interpret(interpreter);

    Value v = cond->interpret(interpreter);
    try
    {
        while (v.dvalue == 1)
        {
            try
            {
                body->interpret(interpreter);
                loop->interpret(interpreter);
                v = cond->interpret(interpreter);
            }
            catch (BreakException &ex)
            {
                loop->interpret(interpreter);
                v = cond->interpret(interpreter);
                if (ex.type == BREAK_TYPE_BREAK)
                    break;
                if (type == BREAK_TYPE_CONTINUE)
                    continue;
            }
        }
    }
    catch (...)
    {
        interpreter->finish_parented_scope();
        throw;
    }
    interpreter->finish_parented_scope();
    return v;
}

void ForNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    throw std::logic_error("Evaluation of for nodes is not supported");
}
