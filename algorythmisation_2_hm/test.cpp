#include "main.cpp"

#include <gtest/gtest.h>


double monteCarloArea(int samples);

TEST(MonteCarloTest, AreaApproximation) {
    double area = monteCarloArea(1000000);
    double expected = 346.0 / 3.0;

    ASSERT_NEAR(area, expected, 1.0); 
}