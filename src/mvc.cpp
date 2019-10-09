#include "mvc.hpp"


void Value_Node::set(Value_Node::value_t _value)
{
    value = _value;
}

Value_Node::value_t Value_Node::get() const
{
    return value;
}
