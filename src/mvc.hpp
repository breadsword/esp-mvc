#ifndef MVC_HPP_INCLUDED
#define MVC_HPP_INCLUDED

#include <functional>
#include <vector>



class Value_Node{
public:
    typedef int value_t;
    typedef std::function<void()> callback_t;

    void register_change_callback(callback_t);
    void set(value_t);
    value_t get() const;
private:
    int value;

    std::vector<callback_t> subscribers;
};

#endif //MVC_HPP_INCLUDED
