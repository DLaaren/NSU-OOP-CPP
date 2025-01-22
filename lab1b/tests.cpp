#include <gtest/gtest.h>
#include "CircularBuffer.h"

TEST(CircularBufferTest, DefaultConstructor)
{
    CircularBuffer<int> buffer;
    
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.get_size(), 0);
    EXPECT_EQ(buffer.get_capacity(), 0);
    EXPECT_EQ(buffer.reserve(), 0);
}

TEST(CircularBufferTest, CopyConstructor)
{
    CircularBuffer<int> buffer1(5);
    buffer1.push_back(1);
    buffer1.push_back(2);
    buffer1.push_back(3);
    
    EXPECT_EQ(buffer1.get_size(), 3);
    EXPECT_EQ(buffer1.front(), 1);
    EXPECT_EQ(buffer1.back(), 3);
    
    CircularBuffer<int> buffer2(buffer1);
    
    EXPECT_EQ(buffer2.get_size(), 3);
    EXPECT_EQ(buffer2.front(), 1);
    EXPECT_EQ(buffer2.back(), 3);
    
    EXPECT_EQ(buffer1, buffer2);
}

TEST(CircularBufferTest, CapacityConstructor)
{
    CircularBuffer<int> buffer(10);
    
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.get_size(), 0);
    EXPECT_EQ(buffer.get_capacity(), 10);
    EXPECT_EQ(buffer.reserve(), 10);
    
    buffer.push_back(1);
    buffer.push_back(2);
    
    EXPECT_EQ(buffer.get_size(), 2);
    EXPECT_EQ(buffer.front(), 1);
    EXPECT_EQ(buffer.back(), 2);
}

TEST(CircularBufferTest, CapacityAndElementConstructor)
{
    CircularBuffer<int> buffer(5, 42);
    
    EXPECT_EQ(buffer.get_size(), 5);
    EXPECT_EQ(buffer.front(), 42);
    EXPECT_EQ(buffer.back(), 42);
    
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(buffer[i], 42);
    }
}

TEST(CircularBufferTest, OperatorAccessNonConst) {
    CircularBuffer<int> cb(5);
    cb.push_back(10);
    cb.push_back(20);
    cb.push_back(30);
    cb[1] = 25;
    EXPECT_EQ(cb[1], 25);
}

TEST(CircularBufferTest, OperatorAccessConst) {
    CircularBuffer<int> cb(5);
    cb.push_back(10);
    cb.push_back(20);
    cb.push_back(30);
    const CircularBuffer<int>& const_cb = cb;
    EXPECT_EQ(const_cb[2], 30);
}

TEST(CircularBufferTest, AtAccessNonConst) {
    CircularBuffer<int> cb(5);
    cb.push_back(10);
    cb.push_back(20);
    cb.push_back(30);
    EXPECT_EQ(cb.at(1), 20);
    EXPECT_THROW(cb.at(5), std::out_of_range);
}

TEST(CircularBufferTest, AtAccessConst) {
    CircularBuffer<int> cb(5);
    cb.push_back(10);
    cb.push_back(20);
    cb.push_back(30);
    const CircularBuffer<int>& const_cb = cb;
    EXPECT_EQ(const_cb.at(2), 30);
    EXPECT_THROW(const_cb.at(5), std::out_of_range);
}

TEST(CircularBufferTest, AtAccessThrowsOnOutOfRange) {
    CircularBuffer<int> cb(5);
    EXPECT_THROW(cb.at(0), std::out_of_range);
    cb.push_back(100);
    EXPECT_EQ(cb.at(0), 100);
    EXPECT_THROW(cb.at(1), std::out_of_range);
}

TEST(CircularBufferTest, SizeFunction) {
    CircularBuffer<int> cb(5);
    EXPECT_EQ(cb.get_size(), 0);
    cb.push_back(5);
    EXPECT_EQ(cb.get_size(), 1);
    cb.push_back(10);
    EXPECT_EQ(cb.get_size(), 2);
}

