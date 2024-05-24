#include <gtest/gtest.h>
#include "../sources/logger.h"

TEST(LoggerTests, Output)
{
    Logger MainLogger;
    MainLogger << std::string("Hello");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}