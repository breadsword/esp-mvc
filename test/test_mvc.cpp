#include <QtTest>

#include "mvc.hpp"

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

/*
    // Test Tree_Model_Node tree setup
    void test_no_parent();
    void test_parent_build_path();
*/

    // Test Value_Node value storage
    void test_set_get_value();
    void test_value_set_notifies();

/*
    // Test Model_Node notification?? -> best on Value_Node
    void test_no_parent_value_int();
    void test_no_parent_value_bool();
    void test_no_parent_value_float();
    void test_no_parent_value_string();
*/
};

class testable_Model_Node : public Model_Node
{
    virtual void build_topic(ostream& s) const override;
};

void testable_Model_Node::build_topic(Model_Node::ostream&) const
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

QTEST_APPLESS_MAIN(test_Value_Node)

#include "test_mvc.moc"
