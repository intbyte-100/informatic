#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <set>

std::set<int> input_set() {
  std::set<int> set;
  int n;
  std::cout << "Enter number of elements: ";
  std::cin >> n;
  std::cout << "Enter " << n << " integers: ";

  for (int i = 0; i < n; ++i) {
    int x;
    std::cin >> x;
    set.insert(x);
  }
  return set;
}

std::set<int> get_union(std::set<int> *a, std::set<int> *b) {
  std::set<int> set_union;
  std::set<int> new_set;
  std::set_union(a->begin(), a->end(), b->begin(), b->end(),
                 std::inserter(set_union, set_union.begin()));

  std::copy_if(set_union.begin(), set_union.end(),
               std::inserter(new_set, new_set.end()),
               [](int x) { return std::abs(x) <= 10; }); 
  
  return new_set;
}

int main() {
  auto a = input_set();
  auto b = input_set();

  for (int i : get_union(&a, &b))
    if (i >= -10 && i <= 10)
      std::cout << i << ' ';

  std::cout << std::endl;
}
