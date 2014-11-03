#include "ValueBindings.h"
#include "NodeVisitor.h"
#include "ValueBinding.h"
#include <algorithm>
USE_SWIFT_NS


ValueBindings::ValueBindings()
    :Declaration(NodeType::ValueBindings)
{
}
ValueBindings::~ValueBindings()
{
}

void ValueBindings::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitValueBindings);
}


void ValueBindings::add(const ValueBindingPtr& var)
{
    valueBindings.push_back(var);
    var->owner = std::static_pointer_cast<ValueBindings>(shared_from_this());
}
void ValueBindings::insertAfter(const ValueBindingPtr& binding, const Iterator& iter)
{
    if(iter == valueBindings.end())
    {
        valueBindings.push_back(binding);
    }
    else
    {
        Iterator it = iter;
        it++;
        valueBindings.insert(it, binding);
    }
    binding->owner = std::static_pointer_cast<ValueBindings>(shared_from_this());
}
ValueBindingPtr ValueBindings::get(int i)
{
    Iterator iter = valueBindings.begin();
    std::advance(iter, i);
    return *iter;
}
int ValueBindings::numBindings()
{
    return valueBindings.size();
}
