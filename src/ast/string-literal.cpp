#include "string-literal.h"
USE_SWIFT_NS


StringLiteral::StringLiteral(const std::wstring& val)
:value(val)
{
}
void StringLiteral::serialize(std::wostream& out)
{
    out<<value;
}