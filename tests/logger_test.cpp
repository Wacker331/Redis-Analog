#include <gtest/gtest.h>
#include "../sources/logger.h"

void LoggerMainThread(Logger *MainLogger)
{
    MainLogger -> LoggerMainThread();
}

TEST(LoggerTests, OneThread)
{
    Logger MainLogger("TestLogs");
    // MainLogger.LoggerMainThread();
    MainLogger << "Hello";
    MainLogger << "test";
    MainLogger << "Protei";
    MainLogger.LoggerMainThread();
}

TEST(LoggerTests, TwoThreads)
{
    Logger MainLogger("TestLogs");
    // MainLogger.LoggerMainThread();
    std::thread Thread1(LoggerMainThread, &MainLogger);
    MainLogger << "Hello";
    MainLogger << "test";
    MainLogger << "Protei";
    Thread1.join();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}