#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>


#include "merge_sort.cpp"
#include "quick_sort.cpp"
#include "radix_sort.cpp"

using namespace std;
using namespace chrono;


vector<int> generate_random_array(size_t size, int min_val = 0, int max_val = 1000000) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(min_val, max_val);
    vector<int> arr(size);
    for (auto& x : arr) x = dist(gen);
    return arr;
}


template<typename SortFunc>
double measure_time(SortFunc sort, const vector<int>& original, const string& name) {
    vector<int> arr = original; 
    auto start = high_resolution_clock::now();
    sort(arr);
    auto end = high_resolution_clock::now();
    duration<double, milli> elapsed = end - start;
    cout << "  " << name << ": " << fixed << setprecision(3) << elapsed.count() << " ms\n";
    return elapsed.count();
}

int main() {
    vector<size_t> sizes;
    for (int i = 0; i < 10; ++i) {
        double size = 1000.0 * pow(10.0, i * (3.0 / 9.0)); 
        sizes.push_back(static_cast<size_t>(round(size)));
    }
   
    
    vector<vector<double>> results(3, vector<double>(sizes.size())); 

    cout << "Performance test on " << sizes.size() << " increasing sizes\n";
    cout << "----------------------------------------------------------\n";

    for (size_t idx = 0; idx < sizes.size(); ++idx) {
        size_t n = sizes[idx];
        cout << "\nSize: " << n << " elements\n";
        vector<int> original = generate_random_array(n);

   
        results[0][idx] = measure_time(merge_sort, original, "Merge sort");

      
        results[1][idx] = measure_time(quick_sort, original, "Quick sort");

     
        results[2][idx] = measure_time(radix_sort, original, "Radix sort");
    }

  
    cout << "\n\nCSV output (size, merge_sort_ms, quick_sort_ms, radix_sort_ms):\n";
    cout << "size,merge_sort,quick_sort,radix_sort\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        cout << sizes[i] << ","
             << fixed << setprecision(3) << results[0][i] << ","
             << results[1][i] << ","
             << results[2][i] << "\n";
    }

    return 0;
}