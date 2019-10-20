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
    QVERIFY(n == "testtopic");
}

void test_Value_Node::test_parent_build_path()
{
    Tree_Model_Node root{"root", nullptr};
    Tree_Model_Node child{"child", &root};

    const auto n = child.notification();
    QVERIFY2(n == "root/child", ("Got "s+n).c_str() );
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
    {
        Value_Model<int> v{5, "test", nullptr};
        const auto n = v.notification();
        QVERIFY2(n == "test 5", ("Got "s+n).c_str() );
    }
    {
        Value_Model<double> v{3.14, "test", nullptr};
        const auto n = v.notification();
        QVERIFY2(n == "test 3.14", ("Got "s+n).c_str() );
    }
    {
        Value_Model<std::string> v{"value", "test", nullptr};
        const auto n = v.notification();
        QVERIFY2(n == "test value", ("Got "s+n).c_str() );
    }
}

void test_Value_Node::test_bool_value_notification()
{
    Value_Model<bool> b{false, "bool_val", nullptr};
    b.set(true);
    const auto b_true = b.notification();
    QVERIFY2(b_true == "bool_val 1", (format("Got: '%1%'")%b_true).str().c_str());

    b.set(false);
    const auto b_false = b.notification();
    QVERIFY2(b_false == "bool_val 0", (format("Got: '%1%'")%b_false).str().c_str());
}

QTEST_APPLESS_MAIN(test_Value_Node)

#include "test_mvc.moc"
