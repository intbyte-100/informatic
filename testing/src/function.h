#pragma once 

#include <cmath>
#include <stdexcept>
struct Result {
    float R;
    float S;
    float C;
};

Result calculation(float x, float y) {
    if (y < 0) throw std::invalid_argument("y must be positive");
    
    Result result = {
        std::pow(x + y, 3 * std::sin(x)),
        std::sqrt(std::abs(x)) / std::log(y),
        0.0f
    };
    
    result.C = std::max(result.S, result.R);
    
    return result;
}