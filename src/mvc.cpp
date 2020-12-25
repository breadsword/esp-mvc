#include "mvc.hpp"
#include "mvc_impl.hpp"
#include <utility>

#include <tuple>

#include <algorithm>

void Model_Node::register_change_callback(callback_t cb)
{
    subscribers.push_back(cb);
}

void Model_Node::notify_subscribers() const
{
    for (auto cb : subscribers)
    {
        cb(*this);
    }
}

std::tuple<string, string> Model_Node::notification() const
{
    ostream topic, value;
    build_topic(topic);
    add_value_string(value);

    return make_tuple(topic.str(), value.str());
}

void Model_Node::add_value_string(ostream &) const
{
}

Tree_Model_Node::Tree_Model_Node(string _topic, Tree_Model_Node *_parent) : Model_Node{}, topic{_topic}, parent{_parent}
{
    if (parent)
    {
        parent->children.push_back(std::ref(*this));
    }
}

bool Tree_Model_Node::operator==(const Tree_Model_Node &rhs) const
{
    return ((this->topic == rhs.topic) && (this->parent == rhs.parent));
}

void Tree_Model_Node::on_tree_execute(std::function<void(Tree_Model_Node &)> f)
{
    f(*this);
    for (auto c : children)
    {
        c.get().on_tree_execute(f);
    }
}

std::vector<string> tokenize(const string path)
{
    std::vector<string> res;
    //    auto word_end = path.begin();
    //    auto word_begin = word_end;

    auto pred = [](char c) { return c == Tree_Model_Node::separator[0]; };

    // Advance to next separator
    auto word_begin = path.begin();

    while (word_begin != path.end())
    {
        word_begin = std::find_if_not(word_begin, path.end(), pred);
        auto word_end = std::find_if(word_begin, path.end(), pred);
        if (word_end != word_begin)
        {
            res.push_back(string{word_begin, word_end});
        }
        word_begin = word_end;
    }

    return res;
}

Tree_Model_Node *Tree_Model_Node::search(string searchtopic)
{
    auto tokens = tokenize(searchtopic);
    return search(tokens.begin(), tokens.end());
}

Tree_Model_Node *Tree_Model_Node::search(std::vector<string>::iterator it, std::vector<string>::iterator end)
{
    if (it == end)
    {
        return this;
    }

    const string child_topic = *it;
    auto child = std::find_if(children.begin(), children.end(), [&child_topic](children_t::value_type c_it) { return c_it.get().topic == child_topic; });
    if (child != children.end())
    {
        return child->get().search(++it, end);
    }

    return nullptr;
}

void Tree_Model_Node::build_topic(ostream &s) const
{
    if (parent != nullptr)
    {
        parent->build_topic(s);
        s << separator;
        s << topic;
    }
}

template <>
void Value_Model<int>::set_from(const string &s)
{
    char *end = nullptr;
    auto val = strtol(s.c_str(), &end, 10);
    if (end != s.c_str())
    {
        set(val);
    }
    else
    {
        set(get());
    }
}

template <>
void Value_Model<double>::set_from(const string &s)
{
    char *end = nullptr;
    auto val = strtod(s.c_str(), &end);
    if (end != s.c_str())
    {
        set(val);
    }
    else
    {
        set(get());
    }
}

template <>
void Value_Model<float>::set_from(const string &s)
{
    char *end = nullptr;
    auto val = strtof(s.c_str(), &end);
    if (end != s.c_str())
    {
        set(val);
    }
    else
    {
        set(get());
    }
}

template <>
void Value_Model<string>::set_from(const string &s)
{
    set(s);
}

template <>
void Value_Model<bool>::set_from(const string &s)
{
    if (s == "0")
    {
        set(false);
    }
    else if (s == "1")
    {
        set(true);
    }
    else
    {
        set(get());
    }
}
