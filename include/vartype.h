#ifndef VARTYPE_H
#define VARTYPE_H
#include <string>
#include "statics.h"
class ClassSystem;
class VarType
{
public:
    VarType();
    virtual ~VarType();
    bool operator==(const VarType &other) const;
    bool operator!=(const VarType &other) const;
    /**
     * Ensures the compatibility between this type and the other type provided
     * \param other The type to ensure compatibility with
     * \param c_system The class system to use for checking for compatibility
     * \return Returns true if both of these types are compatible otherwise false
     */
    bool ensureCompatibility(const VarType& other, ClassSystem* c_system);

    /**
     * Returns true if this VarType represents an array
     */
    bool isArray();
    
    static VarType fromString(std::string value);
    
    VARIABLE_TYPE type;
    std::string value;
    // The array dimensions 0 if this is not an array
    int dimensions;
};

#endif
