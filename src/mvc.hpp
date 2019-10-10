#ifndef MVC_HPP_INCLUDED
#define MVC_HPP_INCLUDED

#include <functional>
#include <vector>
#include <string>

#include <sstream>

// TODO: how to establish a MQTT-like info service for the notification of any Model?
//
// What if Model had a function std::string notification() const, which returned the message including value?
// Then the callback_t would get a pointer to the model and could call notification() on it.
// Better: make notification() something we could put on an iostream.


class Model_Node
{
public:
    Model_Node(std::string _t= "") : topic{_t}
    {}
    virtual ~Model_Node(){}

    typedef std::function<void(const Model_Node&)> callback_t;
    void register_change_callback(callback_t);
    void notify_subscribers() const;

    virtual std::string notification() const;
    const std::string topic;

    std::vector<callback_t> subscribers;
};

class Tree_Model_Node : public Model_Node
{
public:
    Tree_Model_Node(std::string topic = "", Tree_Model_Node *_parent=nullptr) : Model_Node{topic}, parent{_parent}
    {}

    virtual std::string notification() const override;

    constexpr static auto separator = "/";
    void build_topic(std::ostream&) const;

    Tree_Model_Node* parent;
};

template <typename T>
class Value_Model : public Tree_Model_Node{
public:
    typedef T value_t;
    Value_Model(T _value, std::string topic, Tree_Model_Node *parent = nullptr) : Tree_Model_Node{topic, parent}, value{_value}
    {}

    Value_Model() : Tree_Model_Node{}, value{}
    {}

    void set(T);
    T get() const;

    virtual std::string notification() const override;

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
std::string Value_Model<T>::notification() const
{
    std::stringstream s;
    build_topic(s);
    s << " " << value;
    return s.str();
}

#endif //MVC_HPP_INCLUDED
