#include <QtTest>

#include "mvc.hpp"

#include <boost/format.hpp>
using boost::format;

using namespace std::string_literals;

class test_Value_Node : public QObject
{
    Q_OBJECT
public:
    test_Value_Node(){}
    ~test_Value_Node(){}

private slots:
    // Test Model_Node callback mechanism
    void test_callback_object();
    void test_callback_lambda();
    void test_multiple_callbacks();

    // Test Tree_Model_Node tree setup
    void test_no_parent();
    void test_parent_build_path();

    // Test Value_Node value storage
    void test_set_get_value();
    void test_value_set_notifies();

    // Test Value_Node notification
    void test_value_notification();
    void test_bool_value_notification();
    void test_tree_model_no_value_notification();

    // Test tree enumeration
    void test_tree_enumeration();

    // Test tree search
    void test_path_tokenize();
    void test_tree_search_string();

    // Make search and notification consistent
    void test_search_notification();
};

class testable_Model_Node : public Model_Node
{
    virtual void build_topic(ostream& s) const override;
};

void testable_Model_Node::build_topic(ostream&) const
{
}


struct Counting_Callback
{
    void operator()(const Model_Node&)
    {
        ++call_counter;
    }

    bool was_called()
    {
        return call_counter>0;
    }

    int call_counter = 0;
};


void test_Value_Node::test_callback_object()
{
    testable_Model_Node n;
    Counting_Callback callback_by_reference, callback_by_value;

    // This demonstrates how to pass a function object as callback and let the members stay accessible
    n.register_change_callback(std::ref(callback_by_reference));
    n.register_change_callback(callback_by_value);

    n.notify_subscribers();
    QVERIFY(callback_by_reference.was_called());
    QVERIFY(!callback_by_value.was_called());

}

void test_Value_Node::test_callback_lambda()
{
    testable_Model_Node n;
    int call_counter = 0;

    // This shows how to register a lambda as callback
    n.register_change_callback([&call_counter](const Model_Node&){++call_counter;});

    QVERIFY(call_counter == 0);
    n.notify_subscribers();
    QVERIFY(call_counter == 1);
}

void test_Value_Node::test_multiple_callbacks()
{
    testable_Model_Node n;
    int callback_signal = 0;

    n.register_change_callback([&callback_signal](const Model_Node&){callback_signal = 5;});
    n.notify_subscribers();
    QVERIFY(callback_signal == 5);

    n.register_change_callback([&callback_signal](const Model_Node&){++callback_signal;});
    n.notify_subscribers();
    QVERIFY(callback_signal == 6);
}

void test_Value_Node::test_no_parent()
{
    Tree_Model_Node tn{"testtopic", nullptr};
    const auto n = tn.notification();
    const auto res = std::get<0>(n);
    QVERIFY2(res == "", (format("Got: %1%")%res).str().c_str());
}

void test_Value_Node::test_parent_build_path()
{
    Tree_Model_Node root{"root", nullptr};
    Tree_Model_Node child{"child", &root};

    const auto n = child.notification();
    QVERIFY2(std::get<0>(n) == "/child", ("Got "s+std::get<0>(n)).c_str() );
}

void test_Value_Node::test_set_get_value()
{
    Value_Model<int> n;

    const Value_Model<int>::value_t v = 5;
    n.set(v);
    QVERIFY(n.get() == v);

    n.set(v+1);
    QVERIFY(n.get() == v+1);
}

void test_Value_Node::test_value_set_notifies()
{
    Value_Model<int> n;
    int cb_signal = -1;
    n.register_change_callback([&cb_signal](const Model_Node&){cb_signal = 42;});

    QVERIFY(cb_signal != 42);
    n.set(0);
    QVERIFY(cb_signal == 42);
}

void test_Value_Node::test_value_notification()
{
    Tree_Model_Node root{"root", nullptr};
    {
        Value_Model<int> v{5, "test", &root};
        string t, m;
        std::tie(t, m) = v.notification();
        QVERIFY2(t == "/test", ("Got "s+t).c_str() );
        QVERIFY2(m == "5", ("Got "s+m).c_str() );
    }
    {
        Value_Model<double> v{3.14, "test", &root};
        const auto n = v.notification();
        string t, m;
        std::tie(t, m) = v.notification();
        QVERIFY2(t == "/test", ("Got "s+t).c_str() );
        QVERIFY2(m == "3.14", ("Got "s+m).c_str() );
    }
    {
        Value_Model<std::string> v{"value", "test", &root};
        const auto n = v.notification();
        string t, m;
        std::tie(t, m) = v.notification();
        QVERIFY2(t == "/test", ("Got "s+t).c_str() );
        QVERIFY2(m == "value", ("Got "s+m).c_str() );
    }
}

void test_Value_Node::test_bool_value_notification()
{
    Tree_Model_Node root{"root", nullptr};
    Value_Model<bool> b{false, "bool_val", &root};
    {
        b.set(true);
        string t, m;
        std::tie(t, m) = b.notification();
        QVERIFY2(t == "/bool_val", ("Got "s+t).c_str() );
        QVERIFY2(m == "1", ("Got "s+m).c_str() );
    }
    {
        b.set(false);
        const auto b_false = b.notification();
        string t, m;
        std::tie(t, m) = b.notification();
        QVERIFY2(t == "/bool_val", ("Got "s+t).c_str() );
        QVERIFY2(m == "0", ("Got "s+m).c_str() );
    }
}

void test_Value_Node::test_tree_model_no_value_notification()
{
    Tree_Model_Node root{"root", nullptr};
    Tree_Model_Node test{"test", &root};

    string t, m;
    std::tie(t, m) = test.notification();
    QVERIFY2(t == "/test", ("Got "s+t).c_str() );
    QVERIFY2(m == "", ("Got "s+m).c_str() );

}

void test_Value_Node::test_tree_enumeration()
{
    Tree_Model_Node root {"", nullptr};
    Tree_Model_Node c1{"c1", &root}, c2{"c2", &root};
    Tree_Model_Node gc11{"gc11", &c1};

    std::vector<std::reference_wrapper<const Tree_Model_Node>> registry;

    root.on_tree_execute([&registry](const Tree_Model_Node& n){registry.push_back(n);});

    auto it = registry.begin();

    QVERIFY(root == *it);
    ++it;
    QVERIFY(c1 == *it);
    ++it;
    QVERIFY(gc11 == *it);
    ++it;
    QVERIFY(c2 == *it);
}

void test_Value_Node::test_path_tokenize()
{
    {
        const auto tokens = tokenize("");
        QVERIFY(tokens.empty());
    }

    {
        const auto tokens = tokenize("/");
        QVERIFY(tokens.empty());
    }

    {
        const auto tokens = tokenize("root");
        QVERIFY2(tokens.size() == 1, (format("Got: %1%")%tokens.size()).str().c_str());
        QVERIFY2(tokens[0]=="root", (format("Got: %1%")%tokens[0]).str().c_str());
    }

    {
        const auto tokens = tokenize("/root");
        QVERIFY2(tokens.size() == 1, (format("Got: %1%")%tokens.size()).str().c_str());
        QVERIFY2(tokens[0]=="root", (format("Got: %1%")%tokens[0]).str().c_str());
    }

    {
        const auto tokens = tokenize("/root/");
        QVERIFY2(tokens.size() == 1, (format("Got: %1%")%tokens.size()).str().c_str());
        QVERIFY2(tokens[0]=="root", (format("Got: %1%")%tokens[0]).str().c_str());
    }

    {
        const auto tokens = tokenize("root/1/2/3/");
        QVERIFY2(tokens[0]=="root", (format("Got: %1%")%tokens[0]).str().c_str());
        QVERIFY2(tokens.size() == 4, (format("Got: %1%")%tokens.size()).str().c_str());
        QVERIFY2(tokens[1]=="1", (format("Got: %1%")%tokens[1]).str().c_str());
        QVERIFY2(tokens[2]=="2", (format("Got: %1%")%tokens[2]).str().c_str());
        QVERIFY2(tokens[3]=="3", (format("Got: %1%")%tokens[3]).str().c_str());
    }
}

void test_Value_Node::test_tree_search_string()
{
    Tree_Model_Node root {"root", nullptr};
    Tree_Model_Node c1{"c1", &root}, c2{"c2", &root};
    Tree_Model_Node gc11{"gc11", &c1};

//    QFAIL("Not implemented.");
    {
        std::vector<string> tokens {"c1"};
        const auto node = root.search(tokens.begin(), tokens.end());
        QVERIFY(node != nullptr);
        QVERIFY(*node == c1);
    }

    {
        const auto node = root.search("/c1/gc11");
        QVERIFY(node);
        QVERIFY(*node == gc11);
    }
}

void test_Value_Node::test_search_notification()
{
    Tree_Model_Node root {"root", nullptr};
    Tree_Model_Node c1{"c1", &root};

    const string search_string = "/c1";
    const auto node = root.search(search_string);
    QVERIFY(node);

    const auto n = node->notification();
    string topic = std::get<0>(n);
    QVERIFY2(topic == search_string, (format("Topic: %1%, search_str: %2%")%topic%search_string).str().c_str());

}



QTEST_APPLESS_MAIN(test_Value_Node)

#include "test_mvc.moc"
