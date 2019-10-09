#include "mvc.hpp"

void Model::register_change_callback(callback_t cb)
{
    subscribers.push_back(cb);
}

void Model::notify_subscribers() const
{
    for(auto cb : subscribers)
    {
        cb();
    }
}
