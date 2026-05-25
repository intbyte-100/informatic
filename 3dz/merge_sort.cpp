#include <vector>

static void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;            // 1
    int n2 = right - mid;               // 1

    std::vector<int> L(n1);             // 1
    std::vector<int> R(n2);             // 1

    for (int i = 0; i < n1; ++i)        // n1 итераций
        L[i] = arr[left + i];           // 1
    for (int j = 0; j < n2; ++j)        // n2 итераций
        R[j] = arr[mid + 1 + j];        // 1
    
    int i = 0, j = 0, k = left;         // 1
    while (i < n1 && j < n2) {          // не более n1+n2 итераций
        if (L[i] < R[j]) {              // 1 
            arr[k] = L[i];              // 1
            ++i;                        // 1
        } else {
            arr[k] = R[j];              // 1
            ++j;                        // 1
        }
        ++k;                            // 1 
    }

    while (i < n1) {                    // не более n1 итераций
        arr[k] = L[i];                  // 1
        ++i; ++k;                       // 2
    }

    while (j < n2) {                    // не более n2 итераций
        arr[k] = R[j];                  // 1
        ++j; ++k;                       // 2
    }

    // O(n1) + O(n2) + O(n1+n2) = O(n), где n = right-left+1
}

static void merge_sort_rec(std::vector<int>& arr, int left, int right) {
    if (left < right) {                                 // 1
        int mid = left + (right - left) / 2;            // 1
        merge_sort_rec(arr, left, mid);                 // T(n/2)
        merge_sort_rec(arr, mid + 1, right);            // T(n/2)
        merge(arr, left, mid, right);                   // O(n)
    }
    // уравнение: T(n) = 2T(n/2) + O(n)
    // Применяем мастер-теорему:
    // a = 2, b = 2 => по мастер-теореме T(n) = O(n log n) 
}


void merge_sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;        // 1 сравнение
    merge_sort_rec(arr, 0, arr.size() - 1);
}