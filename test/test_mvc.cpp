#include <QtTest>

#include "mvc.hpp"

class test_Value_Node : public QObject
{
    Q_OBJECT
public:
    test_Value_Node(){}
    ~test_Value_Node(){}

private slots:
    void test_callback_object();
    void test_callback_lambda();
    void test_multiple_callbacks();

    void test_set_get_value();
    void test_value_set_notifies();

private:

};

struct Counting_Callback
{
    void operator()(const Model&)
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
    Model n;
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
    Model n;
    int call_counter = 0;

    // This shows how to register a lambda as callback
    n.register_change_callback([&call_counter](const Model&){++call_counter;});

    QVERIFY(call_counter == 0);
    n.notify_subscribers();
    QVERIFY(call_counter == 1);
}

void test_Value_Node::test_multiple_callbacks()
{
    Model n;
    int callback_signal = 0;

    n.register_change_callback([&callback_signal](const Model&){callback_signal = 5;});
    n.notify_subscribers();
    QVERIFY(callback_signal == 5);

    n.register_change_callback([&callback_signal](const Model&){++callback_signal;});
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
    n.register_change_callback([&cb_signal](const Model&){cb_signal = 42;});

    QVERIFY(cb_signal != 42);
    n.set(0);
    QVERIFY(cb_signal == 42);
}

QTEST_APPLESS_MAIN(test_Value_Node)

#include "test_mvc.moc"
