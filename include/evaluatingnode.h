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

#ifndef EVALUATINGNODE_H
#define EVALUATINGNODE_H
#include "node.h"
#include "statics.h"
#include "vartype.h"
#include "extras.h"
class SystemHandler;
class Variable;
struct Evaluation
{
    EVALUATION_TYPE type = 0;
    VarType datatype;
    Variable* variable = NULL;
    struct extras extra;
};

class EvaluatingNode : public Node
{
public:
    EvaluatingNode(NODE_TYPE type);
    virtual ~EvaluatingNode();
    struct Evaluation evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation);
    void evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
protected:
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation) = 0;
};
#endif
