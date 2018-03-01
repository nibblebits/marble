#include "datatype.h"

bool DataType::isPrimitiveDataType(std::string value)
{
    return value == "string" || value == "number" || value == "int" || value == "char" || value == "void";
}
