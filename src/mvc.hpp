#ifndef MVC_HPP_INCLUDED
#define MVC_HPP_INCLUDED

#include <functional>
#include <vector>

class Model
{
  public:
    typedef std::function<void()> callback_t;
    void register_change_callback(callback_t);
    void notify_subscribers() const;

private:
    std::vector<callback_t> subscribers;
};

template <typename T>
class Value_Model : public Model{
public:
    typedef T value_t;
    void set(T);
    T get() const;
private:
    T value;
};

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

#endif //MVC_HPP_INCLUDED
