#include <QtTest>

#include "mvc.hpp"

class test_Value_Node : public QObject
{
    Q_OBJECT
public:
    test_Value_Node(){}
    ~test_Value_Node(){}

private slots:
    void test_set_get_value();
    void test_callback_object();
    void test_callback_lambda();
    void test_multiple_callbacks();

private:

};


void test_Value_Node::test_set_get_value()
{
    Value_Node n;

    const Value_Node::value_t v = 5;
    n.set(v);
    QVERIFY(n.get() == v);

    n.set(v+1);
    QVERIFY(n.get() == v+1);
}

struct Counting_Callback
{
    void operator()()
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
    Value_Node n;
    Counting_Callback callback_by_reference, callback_by_value;

    // This demonstrates how to pass a function object as callback and let the members stay accessible
    n.register_change_callback(std::ref(callback_by_reference));
    n.register_change_callback(callback_by_value);

    n.set(5);
    QVERIFY(callback_by_reference.was_called());
    QVERIFY(!callback_by_value.was_called());

}

void test_Value_Node::test_callback_lambda()
{
    Value_Node n;
    int call_counter = 0;

    // This shows how to register a lambda as callback
    n.register_change_callback([&call_counter]{++call_counter;});

    QVERIFY(call_counter == 0);
    n.set(0);
    QVERIFY(call_counter == 1);
}

void test_Value_Node::test_multiple_callbacks()
{
    Value_Node n;
    int callback_signal = 0;

    n.register_change_callback([&callback_signal]{callback_signal = 5;});
    n.set(0);
    QVERIFY(callback_signal == 5);

    n.register_change_callback([&callback_signal]{++callback_signal;});
    n.set(0);
    QVERIFY(callback_signal == 6);
}


QTEST_APPLESS_MAIN(test_Value_Node)

#include "test_mvc.moc"
