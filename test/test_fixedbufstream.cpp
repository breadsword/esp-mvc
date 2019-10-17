#include <QtTest>

#include "fbstream.hpp"
#include <string>

class test_ostream : public QObject
{
    Q_OBJECT
public:
    test_ostream(){}
    ~test_ostream(){}

private slots:
    void test_instantiate();
    void test_put_string();

};


void test_ostream::test_instantiate()
{
    fixedbufstream fbs;

    QVERIFY(fbs.free_bytes()==1024);
}

void test_ostream::test_put_string()
{
    fixedbufstream fbs;

    fbs << std::string("Test string");

    const auto res = fbs.str();

    QVERIFY2(res == std::string("Test string"), (std::string("Got: '") + res + "'").c_str());

}

QTEST_APPLESS_MAIN(test_ostream)

#include "test_fixedbufstream.moc"
