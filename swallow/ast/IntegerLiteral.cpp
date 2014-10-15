#include "IntegerLiteral.h"
#include "NodeVisitor.h"
USE_SWIFT_NS

IntegerLiteral::IntegerLiteral()
    :Expression(NodeType::IntegerLiteral), valueAsString(L"0"), value(0)
{
}

void IntegerLiteral::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitInteger);
}