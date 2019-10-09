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


QTEST_APPLESS_MAIN(test_Value_Node)

#include "test_mvc.moc"
