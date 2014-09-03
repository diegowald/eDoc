#include <QString>
#include <QtTest>


class TestCORETest : public QObject
{
    Q_OBJECT
    
public:
    TestCORETest();
    
private Q_SLOTS:
    void APITest1();
};

TestCORETest::TestCORETest()
{
}

void TestCORETest::APITest1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestCORETest)

#include "tst_testcoretest.moc"
