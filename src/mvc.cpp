#include "mvc.hpp"

void Model_Node::register_change_callback(callback_t cb)
{
    subscribers.push_back(cb);
}

void Model_Node::notify_subscribers() const
{
    for(auto cb : subscribers)
    {
        cb(*this);
    }
}

std::string Model_Node::notification() const
{
    return topic;
}

std::string Tree_Model_Node::notification() const
{
    std::stringstream s;
    build_topic(s);

    return s.str();
}

void Tree_Model_Node::build_topic(std::ostream& s) const
{
    if (parent !=0)
    {
        parent->build_topic(s);
        s << separator;
    }
    s << topic;
}
