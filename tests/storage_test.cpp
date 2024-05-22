#include <gtest/gtest.h>
#include "../sources/storage.h"

TEST(TestGroupName, InitTest)
{
    Tree MainTree;
    Storage MainStorage(MainTree);
    

}

TEST(TestGroupName, Subtest_2)
{
    ASSERT_FALSE('b' == 'b');
    std::cout << "continue test after failure" << std::endl;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}