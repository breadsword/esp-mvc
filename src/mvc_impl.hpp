#ifndef MVC_IMPL_HPP_INCLUDED
#define MVC_IMPL_HPP_INCLUDED

#include <mvc.hpp>

template <typename T>
void Value_Model<T>::set(T _value)
{
    value = _value;
    notify_subscribers();
}

template <typename T>
T Value_Model<T>::get() const
{
    return value;
}

template <typename T>
void Value_Model<T>::add_value_string(ostream &s) const
{
    s << value;
}

#endif //MVC_IMPL_HPP_INCLUDED
