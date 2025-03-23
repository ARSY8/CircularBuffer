#include <gtest/gtest.h>
#include "CircularBuffer.hpp"

TEST(CircularBufferTest, DefaultConstructor) {
    CircularBuffer cb;
    EXPECT_EQ(cb.size(), 0);
    EXPECT_EQ(cb.capacity(), 10);
    EXPECT_TRUE(cb.empty());
}

TEST(CircularBufferTest, CustomCapacityConstructor) {
    CircularBuffer cb(20);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_EQ(cb.capacity(), 20);
    EXPECT_TRUE(cb.empty());
}

TEST(CircularBufferTest, CopyConstructor) {
    CircularBuffer cb1(5, 42);
    CircularBuffer cb2(cb1);
    EXPECT_EQ(cb2.size(), 5);
    for (int i = 0; i < cb2.size(); ++i) {
        EXPECT_EQ(cb2[i], 42);
    }
}

TEST(CircularBufferTest, AssignmentOperator) {
    CircularBuffer cb1(5, 99);
    CircularBuffer cb2;
    cb2 = cb1;
    EXPECT_EQ(cb2.size(), 5);
    for (int i = 0; i < cb2.size(); ++i) {
        EXPECT_EQ(cb2[i], 99);
    }
}

TEST(CircularBufferTest, PushBackAndFront) {
    CircularBuffer cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_front(0);
    EXPECT_EQ(cb.front(), 0);
    EXPECT_EQ(cb.back(), 2);
    EXPECT_EQ(cb.size(), 3);
}

TEST(CircularBufferTest, PopBackAndFront) {
    CircularBuffer cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.pop_front();
    EXPECT_EQ(cb.front(), 2);
    cb.pop_back();
    EXPECT_EQ(cb.back(), 2);
}

TEST(CircularBufferTest, Resize) {
    // Создаем кольцевой буфер с емкостью 5
    CircularBuffer cb(10);
    
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);
    cb.push_back(5);

    EXPECT_EQ(cb.size(), 5); 

    cb.resize(7, 20);
    EXPECT_EQ(cb.size(), 7);
    EXPECT_EQ(cb[5], 20); 
    EXPECT_EQ(cb[6], 20);   

    cb.resize(3, 0);
    EXPECT_EQ(cb.size(), 3);

    EXPECT_THROW(cb.resize(-1, 0), std::invalid_argument);
}

TEST(CircularBufferTest, AtFunction) {
    CircularBuffer cb(3, 5);
    EXPECT_EQ(cb.at(0), 5);
    EXPECT_EQ(cb.at(2), 5);
    EXPECT_THROW(cb.at(3), std::out_of_range);
}

TEST(CircularBufferTest, InsertAndErase) {
    CircularBuffer cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.insert(1, 99);
    EXPECT_EQ(cb.at(1), 99);
    cb.erase(1, 2);
    EXPECT_EQ(cb.at(1), 3);
}

TEST(CircularBufferTest, LinearizationCheck) {
    CircularBuffer cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_front(4);
    EXPECT_FALSE(cb.is_linearized());
    cb.linearize();
    EXPECT_TRUE(cb.is_linearized());
}

TEST(CircularBufferTest, Swap) {
    CircularBuffer cb1(3, 5);
    CircularBuffer cb2(3, 10);
    cb1.swap(cb2);
    EXPECT_EQ(cb1.at(0), 10);
    EXPECT_EQ(cb2.at(0), 5);
}

TEST(CircularBufferTest, SetCapacity) {
    CircularBuffer cb(5, 3);
    cb.set_capacity(7);
    EXPECT_EQ(cb.capacity(), 7);
    cb.set_capacity(3);
    EXPECT_EQ(cb.capacity(), 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}