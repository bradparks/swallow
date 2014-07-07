
#include "tuple.h"
#include "type-node.h"
USE_SWIFT_NS


Tuple::Tuple()
    :type(NULL)
{
}
Tuple::~Tuple()
{
    SafeDelete(type);
    SafeDeleteAll(elements);
}

void Tuple::serialize(std::wostream& out)
{
    out<<L"(";
    std::vector<Pattern*>::iterator iter = elements.begin();
    for(; iter != elements.end(); iter++)
    {
        (*iter)->serialize(out);
    }
    out<<L")";
}


TypeNode* Tuple::getType()
{
    return type;
}
void Tuple::setType(TypeNode* type)
{
    this->type = type;
}

void Tuple::add(Pattern* st)
{
    elements.push_back(st);
}
int Tuple::numElements()
{
    return elements.size();
}
Pattern* Tuple::getElement(int i)
{
    return elements[i];
}