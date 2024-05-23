#include <gtest/gtest.h>
#include "../sources/storage.h"

TEST(TreeTests, EmptyTreeTest)
{
    Tree MainTree;
    ASSERT_EQ(MainTree.Get("Key").Value, "");
    ASSERT_EQ(MainTree.Get("Key").Key, "");
}

TEST(TreeTests, SimplePut)
{
    Tree MainTree;
    Elem ElemToPut("key1", "value1"), RetElem;

    RetElem = MainTree.Put(ElemToPut);
    ASSERT_EQ(RetElem.Value, "");
    ASSERT_EQ(RetElem.Key, "");

    ASSERT_EQ(MainTree.Put(ElemToPut).Key, "key1");
    ASSERT_EQ(MainTree.Put(ElemToPut).Value, "value1");
}

TEST(TreeTests, DeleteRoot)
{
    Tree MainTree;
    Elem ElemToPut("key1", "value1"), RetElem;

    RetElem = MainTree.Put(ElemToPut);
    RetElem = MainTree.Get("key1");

    ASSERT_EQ(RetElem.Key, "key1");
    ASSERT_EQ(RetElem.Value, "value1");

    RetElem = MainTree.Del("key1");
    ASSERT_EQ(RetElem.Key, "key1");
    ASSERT_EQ(RetElem.Value, "value1");

    RetElem = MainTree.Get("key1");
    ASSERT_EQ(RetElem.Key, "");
    ASSERT_EQ(RetElem.Value, "");
}

TEST(TreeTests, Delete1Child)
{
    Tree MainTree;
    Elem RetElem;

    RetElem = MainTree.Put(Elem("abc", "value1"));
    RetElem = MainTree.Put(Elem("bcd", "value2"));
    ASSERT_EQ(RetElem.Key, "");
    ASSERT_EQ(RetElem.Value, "");

    RetElem = MainTree.Del("bcd");
    ASSERT_EQ(RetElem.Key, "bcd");
    ASSERT_EQ(RetElem.Value, "value2");

    RetElem = MainTree.Get("abc");
    ASSERT_EQ(RetElem.Key, "abc");
    ASSERT_EQ(RetElem.Value, "value1");
}

TEST(TreeTests, Delete2Childs)
{
    Tree MainTree;
    Elem RetElem;

    RetElem = MainTree.Put(Elem("bcd", "value1"));
    ASSERT_EQ(RetElem.Key, "");
    RetElem = MainTree.Put(Elem("abc", "value2"));
    ASSERT_EQ(RetElem.Key, "");
    RetElem = MainTree.Put(Elem("cde", "value3"));
    ASSERT_EQ(RetElem.Key, "");

    RetElem = MainTree.Del("bcd");
    ASSERT_EQ(RetElem.Key, "bcd");
    ASSERT_EQ(RetElem.Value, "value1");

    RetElem = MainTree.Get("bcd");
    ASSERT_EQ(RetElem.Key, "");
    ASSERT_EQ(RetElem.Value, "");

    RetElem = MainTree.Get("abc");
    ASSERT_EQ(RetElem.Key, "abc");
    ASSERT_EQ(RetElem.Value, "value2");

    RetElem = MainTree.Get("cde");
    ASSERT_EQ(RetElem.Key, "cde");
    ASSERT_EQ(RetElem.Value, "value3");

}

TEST(TreeTests, BigTreeTest)
{
    Tree MainTree;
    Elem RetElem;

    RetElem = MainTree.Put(Elem("bcd", "value1"));
    ASSERT_EQ(RetElem.Value, "");
    RetElem = MainTree.Put(Elem("abc", "value2"));
    ASSERT_EQ(RetElem.Value, "");
    RetElem = MainTree.Put(Elem("cde", "value3"));
    ASSERT_EQ(RetElem.Value, "");
    RetElem = MainTree.Put(Elem("def", "value4"));
    ASSERT_EQ(RetElem.Value, "");
    RetElem = MainTree.Put(Elem("efg", "value5"));
    ASSERT_EQ(RetElem.Value, "");
    RetElem = MainTree.Put(Elem("fgh", "value6"));
    ASSERT_EQ(RetElem.Value, "");
    RetElem = MainTree.Put(Elem("ghi", "value7"));
    ASSERT_EQ(RetElem.Value, "");

    RetElem = MainTree.Get("bcd");
    ASSERT_EQ(RetElem.Value, "value1");

    RetElem = MainTree.Del("def");
    ASSERT_EQ(RetElem.Value, "value4");

    RetElem = MainTree.Get("def");
    ASSERT_EQ(RetElem.Value, "");

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}