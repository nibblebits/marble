#ifndef SINGLEFUNCTION_H
#define SINGLEFUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
#include "object.h"
#include "vartype.h"
#include "function.h"

class SingleFunction : public Function
{
    public:
        SingleFunction(SystemHandler* handler, FUNCTION_TYPE type, std::string name, std::vector<VarType> argument_types, VarType return_type);
        virtual ~SingleFunction();
        std::vector<VarType> argument_types;
        VarType return_type;
};
#endif
