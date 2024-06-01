#include "../sources/server.h"
#include <gtest/gtest.h>

TEST(ClientTests, ParseInputTest)
{
    Tree MainTree;
    Storage MainStorage(MainTree);
    Logger MainLogger;
    struct sockaddr_in Addr;
    int Sock = 0;
    Client TestClient(Sock, Addr, MainStorage, MainLogger);
    char buffer[1024];
    strcpy(buffer, "GET key       value\x0A");
    std::vector<std::string> RetVector = TestClient.ParseInput(buffer);

    ASSERT_EQ(RetVector.size(), 3);
    ASSERT_EQ(RetVector[0], "GET");
    ASSERT_EQ(RetVector[1], "key");
    ASSERT_EQ(RetVector[2], "value");
}

TEST(ClientTests, HandlerTest)
{
    Tree MainTree;
    Storage MainStorage(MainTree);
    Logger MainLogger;
    struct sockaddr_in Addr;
    int Sock = 0;
    Client TestClient(Sock, Addr, MainStorage, MainLogger);
    Elem TestElem;
    char buffer[1024] = "GET key       value\x0A";

    TestClient.RequestHandler(buffer);
    ASSERT_EQ(std::string(buffer), "NE");

    strcpy(buffer, "PUT key1 value1\n");
    TestClient.RequestHandler(buffer);
    ASSERT_EQ(std::string(buffer), "");

    strcpy(buffer, "GET key1    \n");
    TestClient.RequestHandler(buffer);
    ASSERT_EQ(std::string(buffer), "value1");

    strcpy(buffer, "DEL key1\n");
    TestClient.RequestHandler(buffer);
    ASSERT_EQ(std::string(buffer), "value1");

    strcpy(buffer, "GET key1    \n");
    TestClient.RequestHandler(buffer);
    ASSERT_EQ(std::string(buffer), "NE");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
