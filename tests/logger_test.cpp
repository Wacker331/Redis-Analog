#include <gtest/gtest.h>
#include "../sources/logger.h"

TEST(LoggerTests, TwoThreads)
{
    Logger MainLogger("TestLogs");
    std::thread Thread1(InitLogger, &MainLogger);
    MainLogger << "Hello";
    MainLogger << "test";
    MainLogger << "Protei";
    sleep(2);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}