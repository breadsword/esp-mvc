#include <QtTest>

#include "fbstream.hpp"
#include <string>

#include <boost/format.hpp>
using boost::format;

class test_fixedbufstream : public QObject
{
    Q_OBJECT
public:
    test_fixedbufstream(){}
    ~test_fixedbufstream(){}

private slots:
    void test_free_bytes();
    void test_put_string();
    void test_put_int();
    void test_put_float();
    void test_isfull();

};


void test_fixedbufstream::test_free_bytes()
{
    fixedbufstream<128> fbs;

    QVERIFY(fbs.free_bytes()==128);

    fbs.putc('1');
    QVERIFY2(fbs.free_bytes()==127, (format("Got: %1%")%fbs.free_bytes()).str().c_str());
}

void test_fixedbufstream::test_put_string()
{
    fixedbufstream<1024> fbs;

    fbs << std::string("Test string");

    const auto res = fbs.str();
    QVERIFY2(res == std::string("Test string"), (std::string("Got: '") + res + "'").c_str());

}

void test_fixedbufstream::test_isfull()
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

void test_fixedbufstream::test_put_int()
{
    fixedbufstream<30> fbs;
    {
        fbs << 5;
        const auto res = fbs.str();
        QVERIFY2(res == "5", (format("Got: '%1%'")%res).str().c_str());
    }
    {
        fbs << 10;
        const auto res = fbs.str();
        QVERIFY2(res == "510", (format("Got: '%1%'")%res).str().c_str());
    }
    {
        fixedbufstream<16> fb;
        fb << -3;
        const auto res = fb.str();
        QVERIFY2(res == "-3", (format("Got: '%1%'")%res).str().c_str());
    }
}

void test_fixedbufstream::test_put_float()
{
    fixedbufstream<30> fbs;

    {
        fbs << 2.5F;
        const auto res = fbs.str();
        QVERIFY2(res == "2.50", (format("Got: '%1%'")%res).str().c_str());
    }

    {
        fbs << 2.12345;
        const auto res = fbs.str();
        QVERIFY2(res == "2.502.12", (format("Got: '%1%'")%res).str().c_str());
    }
    {
        fixedbufstream<30> fbs;
        fbs << 2.126;
        const auto res = fbs.str();
        QVERIFY2(res == "2.13", (format("Got: '%1%'")%res).str().c_str());
    }

    {
        fixedbufstream<16> fb;
        fb << -3.0;
        const auto res = fb.str();
        QVERIFY2(res == "-3.00", (format("Got: '%1%'")%res).str().c_str());
    }

    {
        fixedbufstream<16> fb;
        fb << -3.126;
        const auto res = fb.str();
        QVERIFY2(res == "-3.13", (format("Got: '%1%'")%res).str().c_str());
    }
}

QTEST_APPLESS_MAIN(test_fixedbufstream)

#include "test_fixedbufstream.moc"
