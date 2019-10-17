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

string Model_Node::notification() const
{
    std::stringstream s;
    build_topic(s);
    add_value_string(s);

    return s.str();
}

void Model_Node::add_value_string(ostream &) const
{}

void Tree_Model_Node::build_topic(ostream& s) const
{
    if (parent != nullptr)
    {
        parent->build_topic(s);
        s << separator;
    }
    s << topic;
}
