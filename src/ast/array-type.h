#ifndef ARRAY_TYPE_H
#define ARRAY_TYPE_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class ArrayType : public TypeNode
{
public:
    ArrayType();
public:
    void setInnerType(TypeNode* innerType);
    TypeNode* getInnerType();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//ARRAY_TYPE_H
