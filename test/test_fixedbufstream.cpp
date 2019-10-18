#include <QtTest>

#include "fbstream.hpp"
#include <string>

#include <boost/format.hpp>
using boost::format;

class test_ostream : public QObject
{
    Q_OBJECT
public:
    test_ostream(){}
    ~test_ostream(){}

private slots:
    void test_free_bytes();
    void test_put_string();
    void test_isfull();

};


void test_ostream::test_free_bytes()
{
    fixedbufstream<128> fbs;

    QVERIFY(fbs.free_bytes()==128);

    fbs.putc('1');
    QVERIFY2(fbs.free_bytes()==127, (format("Got: %1%")%fbs.free_bytes()).str().c_str());
}

void test_ostream::test_put_string()
{
    fixedbufstream<1024> fbs;

    fbs << std::string("Test string");

    const auto res = fbs.str();
    QVERIFY2(res == std::string("Test string"), (std::string("Got: '") + res + "'").c_str());

}

void test_ostream::test_isfull()
{
    fixedbufstream<4> fbs;
    QVERIFY(fbs);
    fbs << "1234";
    QVERIFY(fbs);
    fbs.putc('5');
    QVERIFY(!fbs);

    const auto res = fbs.str();
    QVERIFY2(res == std::string("1234"), (std::string("Got: '") + res + "'").c_str());
}

QTEST_APPLESS_MAIN(test_ostream)

#include "test_fixedbufstream.moc"
