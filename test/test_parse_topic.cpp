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
    QVERIFY_EXCEPTION_THROWN(parse_topic(string{}), topic_parse_error);

    QVERIFY_EXCEPTION_THROWN(parse_topic(string{"/"}), topic_parse_error);
    QVERIFY_EXCEPTION_THROWN(parse_topic(string{"//"}), topic_parse_error);
    QVERIFY_EXCEPTION_THROWN(parse_topic(string{"/host"}), topic_parse_error);
    QVERIFY_EXCEPTION_THROWN(parse_topic(string{"/host/dir"}), topic_parse_error);
    QVERIFY_EXCEPTION_THROWN(parse_topic(string{"/host/dir/"}), topic_parse_error);

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
        QVERIFY2(s=="topic/", (format("Got: '%1%'")%s).str().c_str());
    }
}

QTEST_APPLESS_MAIN(test_parse_topic)

#include "test_parse_topic.moc"
