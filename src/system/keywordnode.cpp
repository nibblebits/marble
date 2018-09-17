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

#include "keywordnode.h"
#include "statics.h"
#include "datatype.h"
#include "variable.h"

KeywordNode::KeywordNode() : EvaluatingNode(NODE_TYPE_KEYWORD)
{
    this->value = "";
}

KeywordNode::~KeywordNode()
{

}

bool KeywordNode::isDataTypeKeyword()
{
    // Keywords can only represent primitive datatypes.
    return DataType::isPrimitiveDataType(this->value);
}

void KeywordNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    // The keyword node only evaluates data types.
    if (!(expected_evaluation & EVALUATION_TYPE_DATATYPE))
        return;
    
    if (!isDataTypeKeyword())
        throw std::logic_error("Not a datatype keyword.");
        
    evaluation->type |= EVALUATION_TYPE_DATATYPE;
    evaluation->datatype.type = Variable::getVariableTypeForString(this->value);
    evaluation->datatype.value = this->value;
}
