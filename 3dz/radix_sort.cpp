#include <vector>
#include <algorithm>

void bucket_sort_by_digit(std::vector<int>& arr, int exp) {
    const int base = 10;                               // 1
    std::vector<std::vector<int>> buckets(base);       // 1

    for (int x : arr) {                                // n итераций
        int digit = (x / exp) % base;                  // 1 
        buckets[digit].push_back(x);                   // 1 
    }

    arr.clear();                                       // 1
    for (auto& bucket : buckets) {                     // base = 10 итераций 
        arr.insert(arr.end(), bucket.begin(), bucket.end()); // суммарно O(n) по всем корзинам
    }
    // Сложность bucket_sort_by_digit: O(n) 
}

void radix_sort(std::vector<int>& arr) {
    if (arr.empty()) return;                           // 1 

    int max_val = *std::max_element(arr.begin(), arr.end()); // O(n)

    for (int exp = 1; max_val / exp > 0; exp *= 10) {  // d итераций (d = число разрядов)
        bucket_sort_by_digit(arr, exp);                // O(n) 
    }
    // T(n) = d * O(n) = O(d * n) 
}