#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <iomanip>
#include <future>

const int N = 1000;

int partition(std::vector<int>& arr, int low, int high) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);
    int random_idx = dis(gen);
    std::swap(arr[random_idx], arr[high]);

    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void sequential_quicksort(std::vector<int>& arr, int low, int high) {
    if (low >= high) return;
    int pivot_idx = partition(arr, low, high);
    sequential_quicksort(arr, low, pivot_idx - 1);
    sequential_quicksort(arr, pivot_idx + 1, high);
}

void parallel_quicksort(std::vector<int>& arr, int low, int high, int depth) {
    if (low >= high) return;

    int size = high - low + 1;
    if (size <= N || depth <= 0) {
        sequential_quicksort(arr, low, high);
        return;
    }

    int pivot_idx = partition(arr, low, high);
    int next_depth = depth - 1;

    auto left = std::async(std::launch::async,
        parallel_quicksort,
        std::ref(arr),
        low,
        pivot_idx - 1,
        next_depth
    );

    parallel_quicksort(arr, pivot_idx + 1, high, next_depth);
    left.get();
}

void quicksort_sync(std::vector<int>& arr) {
    if (arr.empty()) return;
    sequential_quicksort(arr, 0, arr.size() - 1);
}

void quicksort_async(std::vector<int>& arr, int num_threads) {
    if (arr.empty()) return;

    int depth = 0;
    while ((1 << depth) < num_threads) depth++;

    parallel_quicksort(arr, 0, arr.size() - 1, depth);
}

bool is_sorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i)
        if (arr[i] < arr[i - 1]) return false;
    return true;
}

std::vector<int> generate_random_array(int n) {
    std::vector<int> arr(n);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, n);
    for (int i = 0; i < n; ++i)
        arr[i] = dis(gen);
    return arr;
}

double measure_sync(const std::vector<int>& original) {
    std::vector<int> arr = original;
    auto start = std::chrono::high_resolution_clock::now();
    quicksort_sync(arr);
    auto end = std::chrono::high_resolution_clock::now();
    if (!is_sorted(arr)) return -1;
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

double measure_async(const std::vector<int>& original, int num_threads) {
    std::vector<int> arr = original;
    auto start = std::chrono::high_resolution_clock::now();
    quicksort_async(arr, num_threads);
    auto end = std::chrono::high_resolution_clock::now();
    if (!is_sorted(arr)) return -1;
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main() {
    std::vector<int> sizes = {100000, 500000, 1000000, 5000000, 10000000, 20000000};
    std::vector<int> thread_counts = {2, 4};

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Array size\tSync time (s)\t";

    for (int tc : thread_counts) {
        std::cout << "Async " << tc << " (s)\tSpeedup " << tc << "\t";
    }

    std::cout << std::endl;

    for (int n : sizes) {
        std::cout << n << "\t\t";
        std::vector<int> original = generate_random_array(n);

        double sync_time = measure_sync(original);
        if (sync_time < 0) {
            std::cout << "error\t\t";
            for (size_t i = 0; i < thread_counts.size(); ++i)
                std::cout << "-\t\t-\t\t";
            std::cout << std::endl;
            continue;
        }

        std::cout << sync_time << "\t\t";

        for (int tc : thread_counts) {
            double async_time = measure_async(original, tc);
            if (async_time < 0) {
                std::cout << "-\t\t-\t\t";
            } else {
                double speedup = sync_time / async_time;
                std::cout << async_time << "\t\t" << speedup << "\t\t";
            }
        }

        std::cout << std::endl;
    }

    return 0;
}