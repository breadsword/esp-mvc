#include <QtTest>

#include "mvc.hpp"

class test_mvc : public QObject
{
    Q_OBJECT
public:
    test_mvc(){}
    ~test_mvc(){}

private slots:
    void test_set_temperature();

private:

};


void test_mvc::test_set_temperature()
{
    QFAIL("Test not implemented.");
}

QTEST_APPLESS_MAIN(test_mvc)

#include "test_mvc.moc"
