#include <QtTest>

#include "parse_topic.hpp"

#include <boost/format.hpp>
using boost::format;

class test_parse_topic : public QObject
{
    Q_OBJECT
public:
    test_parse_topic(){}
    ~test_parse_topic(){}

private slots:
    void test_empty_string();
};



void test_parse_topic::test_empty_string()
{
    {
        const auto s = parse_topic(string{});
        QVERIFY2(s=="", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic(string{"/"});
        QVERIFY2(s=="", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic(string{"//"});
        QVERIFY2(s=="", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic(string{"/host"});
        QVERIFY2(s=="", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic(string{"/host/dir"});
        QVERIFY2(s=="", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic(string{"/host/dir/"});
        QVERIFY2(s=="", (format("Got: '%1%'")%s).str().c_str());
    }

    {
        const auto s = parse_topic({"/wemos/o/topic"});
        QVERIFY2(s=="topic", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic({"wemos/o/topic"});
        QVERIFY2(s=="topic", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic({"wemos/o/topic/"});
        QVERIFY2(s=="topic", (format("Got: '%1%'")%s).str().c_str());
    }
    {
        const auto s = parse_topic({"wemos/o/topic/lala"});
        QVERIFY2(s=="topic/lala", (format("Got: '%1%'")%s).str().c_str());
    }


}

QTEST_APPLESS_MAIN(test_parse_topic)

#include "test_parse_topic.moc"
