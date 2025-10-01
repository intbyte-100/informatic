#include <cmath>
#include <iostream>
#include "cstdlib"

int main() {
  float x, y;

  std::cin >> x >> y;
  float R = std::pow(x + y, 3 * std::sin(x));
  float S = std::sqrt(std::abs(x)) / std::log(M_E);

  std::cout << "R = " << R << std::endl;
  std::cout << "S = " << S << std::endl;

  float C = std::max(R, S);
  
  std::cout << "C = " << C << std::endl;
  system("pause");
}
