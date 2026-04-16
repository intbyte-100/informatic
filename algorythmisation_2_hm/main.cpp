#include <iostream>
#include <random>

double f1(double x) {
    return 2 * x * x + 1;
}

double f2(double x) {
    return -2 * x * x + 7;
}

double monteCarloArea(int samples) {
    double x_min = 5.0;
    double x_max = 6.0;


    double y_min = f2(6); 
    double y_max = f1(6); 

    std::mt19937 gen(67);
    std::uniform_real_distribution<> dx(x_min, x_max);
    std::uniform_real_distribution<> dy(y_min, y_max);

    int inside = 0;

    for (int i = 0; i < samples; ++i) {
        double x = dx(gen);
        double y = dy(gen);

        double y_low = std::min(f1(x), f2(x));
        double y_high = std::max(f1(x), f2(x));

        if (y >= y_low && y <= y_high) {
            inside++;
        }
    }

    double rect_area = (x_max - x_min) * (y_max - y_min);
    return rect_area * (double)inside / samples;
}

int main() {
    long long samples;
    std::cin >> samples;
    std::cout << monteCarloArea(samples);
}