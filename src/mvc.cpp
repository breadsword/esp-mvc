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

std::pair<string, string> Model_Node::notification() const
{
    ostream topic, value;
    build_topic(topic);
    add_value_string(value);

    return make_pair(topic.str(), value.str());
}

void Model_Node::add_value_string(ostream &) const
{}

Tree_Model_Node::Tree_Model_Node(string _topic, Tree_Model_Node *_parent) : Model_Node{}, topic{_topic}, parent{_parent}
{
    if(parent)
    {
        parent->children.push_back(std::ref(*this));
    }
}

bool Tree_Model_Node::operator==(const Tree_Model_Node &rhs) const
{
    return ((this->topic == rhs.topic) && (this->parent == rhs.parent));
}

void Tree_Model_Node::on_tree_execute(std::function<void (const Tree_Model_Node &)> f) const
{
    f(*this);
    for (auto c : children)
    {
        c.get().on_tree_execute(f);
    }
}

void Tree_Model_Node::build_topic(ostream& s) const
{
    if (parent != nullptr)
    {
        parent->build_topic(s);
        s << separator;
    }
    s << topic;
}
