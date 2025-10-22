#include <algorithm>
#include <iostream>
#include <vector>

void input_vector(std::vector<int> *vec) {
  std::cout << "Enter " << vec->size() << " integers: ";
  for (int i = 0; i < vec->size(); i++) {
    std::cin >> (*vec)[i];
  }
}

int count_negative(std::vector<int> *vec) {
  return std::count_if(vec->begin(), vec->end(),
                       [](int number) { return number < 0; });
}

std::vector<int> *compare_and_return(std::vector<int> *a, std::vector<int> *b) {
  return count_negative(a) > count_negative(b) ? a : b;
}

int main() {
  std::vector<int> a(5);
  std::vector<int> b(5);

  input_vector(&a);
  input_vector(&b);

  for (auto i : *compare_and_return(&a, &b))
    std::cout << i << " ";

  std::cout << std::endl;
}
