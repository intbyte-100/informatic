#include <vector>
#include <algorithm>

static int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];                     // 1
    int i = low - 1;                           // 1

    for (int j = low; j < high; ++j) {         // (high-low) итераций 
        if (arr[j] <= pivot) {                 // 1 
            ++i;                               // 1
            std::swap(arr[i], arr[j]);         // 1
        }
    }
    std::swap(arr[i + 1], arr[high]);          // 1
    return i + 1;                              // 1
    // Сложность partition: O(high-low) = O(n) 
}

// Рекурсивная быстрая сортировка
static void quick_sort_rec(std::vector<int>& arr, int low, int high) {
    if (low < high) {                          // 1 сравнение
        int pi = partition(arr, low, high);    // O(m), где m = high-low+1
        // Рекурсивные вызовы:
        quick_sort_rec(arr, low, pi - 1);      // T(k), где k = pi-1 - low
        quick_sort_rec(arr, pi + 1, high);     // T(n-k-1), n = high-low+1
    }
    // T(n) = T(k) + T(n-k-1) + O(n)
    // В лучшем и среднем случаях k = n/2, тогда T(n) = 2T(n/2) + O(n)
    // Применяем мастер-теорему: a=2, b=2
    // n^(log_b a) = n, f(n)=O(n) → случай 2 → T(n) = O(n log n)
    // В худшем случае (уже отсортированный массив) k = 0 или k = n-1,
    // получаем T(n) = T(n-1) + O(n) → O(n^2)
}

void quick_sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;              // 1
    quick_sort_rec(arr, 0, arr.size() - 1);
}