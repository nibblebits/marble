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

#include "outputfilternode.h"
#include "validator.h"
#include "interpreter.h"
#include "expnode.h"
#include "bodynode.h"
#include "varnode.h"
OutputFilterNode::OutputFilterNode() : Statement(NODE_TYPE_OUTPUT_FILTER)
{
    this->filter_variable = NULL;
    this->output_body = NULL;
    this->filter_body = NULL;
}

OutputFilterNode::~OutputFilterNode()
{

}

void OutputFilterNode::test(Validator* validator, struct extras extra)
{
    this->filter_body->test(validator);
    validator->expecting("string");
    this->filter_variable->test(validator);
    validator->endExpecting();
    this->output_body->test(validator);
}

Value OutputFilterNode::interpret(Interpreter* interpreter, struct extras extra)
{
    std::string output_data = "";
    interpreter->setOutputFunction([&](const char* data, size_t length) -> void {
        for (int i = 0; i < length; i++)
            output_data += data[i];
    });
    this->filter_body->interpret(interpreter);
    interpreter->finishOutputFunction();

    interpreter->new_parented_scope();
    try
    {
        Value filter_var = this->filter_variable->interpret(interpreter);
        filter_var.holder->value.set(output_data);
        this->output_body->interpret(interpreter);
    }
    catch(...)
    {
        interpreter->finish_parented_scope();
        Value v;
        return v;
    }

    interpreter->finish_parented_scope();


    Value v;
    return v;
}

void OutputFilterNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
