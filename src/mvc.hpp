#ifndef MVC_HPP_INCLUDED
#define MVC_HPP_INCLUDED

#include <functional>
#include <vector>
#include "fbstream.hpp"
#include "types.hpp"
#include <cstdlib>

// TODO: make notification() something we could put on an iostream as in
// std::cout << m.notification()
// and perform the output using expression templates

class Model_Node
{
public:
    Model_Node()
    {
    }
    virtual ~Model_Node() {}

    typedef std::function<void(const Model_Node &)> callback_t;
    void register_change_callback(callback_t);
    void notify_subscribers() const;

    // notification uses algorithm pattern
    std::tuple<string, string> notification() const;
    std::vector<callback_t> subscribers;

protected:
    typedef fixedbufstream<128> ostream;
    // make these function protected to allow changing the std::ostream interface
    virtual void build_topic(ostream &) const = 0;
    virtual void add_value_string(ostream &) const;
};

class Tree_Model_Node : public Model_Node
{
public:
    Tree_Model_Node(string _topic = "", Tree_Model_Node *_parent = nullptr);

    constexpr static auto separator = "/";
    const string topic;
    Tree_Model_Node *parent;

    typedef std::vector<std::reference_wrapper<Tree_Model_Node>> children_t;
    children_t children;

    bool operator==(const Tree_Model_Node &rhs) const;

    void on_tree_execute(std::function<void(Tree_Model_Node &)> f);

    Tree_Model_Node *search(string searchtopic);
    Tree_Model_Node *search(std::vector<string>::iterator it, std::vector<string>::iterator end);

    virtual void set_from(const string &) {}

protected:
    virtual void build_topic(ostream &) const override;
};

template <typename T>
class Value_Model : public Tree_Model_Node
{
public:
    typedef T value_t;
    Value_Model(T _value, string _topic, Tree_Model_Node *_parent = nullptr) : Tree_Model_Node{_topic, _parent}, value{_value}
    {
    }

    Value_Model() : Tree_Model_Node{}, value{}
    {
    }

    virtual void set_from(const string &) override;
    void set(T);
    T get() const;

protected:
    virtual void add_value_string(ostream &) const override;

private:
    T value;
};

std::vector<string> tokenize(const string path);

#endif //MVC_HPP_INCLUDED
