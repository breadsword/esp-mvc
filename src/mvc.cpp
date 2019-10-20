#include "mvc.hpp"
#include <utility>

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

std::pair<string, string> Model_Node::notification() const
{
    ostream topic, value;
    build_topic(topic);
    add_value_string(value);

    return std::make_pair(topic.str(), value.str());
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
