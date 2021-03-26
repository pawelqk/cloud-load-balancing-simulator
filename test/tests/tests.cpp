#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(Test, Passing)
{
    ASSERT_TRUE(true);
}

TEST(Test, Failing)
{
    ASSERT_EQ(1, 2);
}
