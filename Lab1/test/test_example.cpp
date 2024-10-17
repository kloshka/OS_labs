#include <gtest/gtest.h>


TEST(SumTest, BasicAssertions) {
    float a = 1.2, b = 2.3;
    EXPECT_FLOAT_EQ(a + b, 3.5);
}


