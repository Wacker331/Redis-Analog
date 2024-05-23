add_executable(StorageTest storage_test.cpp)
target_link_libraries(StorageTest storage gtest gtest_main)

add_test(StorageTest StorageTest)