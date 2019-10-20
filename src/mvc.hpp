#ifndef MVC_HPP_INCLUDED
#define MVC_HPP_INCLUDED

#include <functional>
#include <vector>
#include "fbstream.hpp"
#include "types.hpp"

// TODO: make notification() something we could put on an iostream as in
// std::cout << m.notification()
// and perform the output using expression templates


class Model_Node
{
public:
    Model_Node()
    {}
    virtual ~Model_Node(){}

    typedef std::function<void(const Model_Node&)> callback_t;
    void register_change_callback(callback_t);
    void notify_subscribers() const;

    // notification uses algorithm pattern
    string notification() const;
    std::vector<callback_t> subscribers;

protected:
    typedef fixedbufstream<64> ostream;
    // make these function protected to allow changing the std::ostream interface
    virtual void build_topic(ostream&) const = 0;
    virtual void add_value_string(ostream&) const;
};

class Tree_Model_Node : public Model_Node
{
public:
    Tree_Model_Node(string _topic = "", Tree_Model_Node *_parent=nullptr) : Model_Node{}, topic{_topic}, parent{_parent}
    {}

    constexpr static auto separator = "/";
    const string topic;
    Tree_Model_Node* parent;

protected:
    virtual void build_topic(ostream&) const override;
};

template <typename T>
class Value_Model : public Tree_Model_Node{
public:
    typedef T value_t;
    Value_Model(T _value, string _topic, Tree_Model_Node *_parent = nullptr) : Tree_Model_Node{_topic, _parent}, value{_value}
    {}

    Value_Model() : Tree_Model_Node{}, value{}
    {}

    void set(T);
    T get() const;

protected:
    virtual void add_value_string(ostream&) const override;

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

template<typename T>
void Value_Model<T>::add_value_string(ostream& s) const
{
    s << " " << value;
}

#endif //MVC_HPP_INCLUDED
