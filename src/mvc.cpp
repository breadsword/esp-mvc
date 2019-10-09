#include "mvc.hpp"


void Value_Node::set(Value_Node::value_t _value)
{
    value = _value;

    for(auto cb : subscribers)
    {
        cb();
    }
}

Value_Node::value_t Value_Node::get() const
{
    return value;
}

void Value_Node::register_change_callback(callback_t cb)
{
    subscribers.push_back(cb);
}
