#include <gtest/gtest.h>
#include <stdexcept>
#include "function.h"

TEST(Function, Calculation) {
    auto result = calculation(23, 23);
    EXPECT_FLOAT_EQ(result.S, 1.52953f);
    EXPECT_FLOAT_EQ(result.C, 1.52953f);
}


TEST(Function, FailOnInvalidArguments) {
    EXPECT_THROW(calculation(23, -1), std::invalid_argument);
}