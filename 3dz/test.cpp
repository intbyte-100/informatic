#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>

#include "merge_sort.cpp"
#include "radix_sort.cpp"
#include "quick_sort.cpp"

static std::vector<int> generate_random_array(size_t size, int min_val = 0, int max_val = 1000000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min_val, max_val);
    std::vector<int> arr(size);
    for (auto& x : arr) x = dist(gen);
    return arr;
}


template<typename SortFunc>
void test_sorting(SortFunc sort, const std::string& name) {
    std::vector<int> empty;
    sort(empty);
    EXPECT_TRUE(empty.empty()) << name << " failed on empty array";

    std::vector<int> single = {42};
    sort(single);
    EXPECT_EQ(single, std::vector<int>({42})) << name << " failed on single element";

    auto arr = generate_random_array(1000);
    auto expected = arr;
    std::sort(expected.begin(), expected.end());
    sort(arr);
    EXPECT_EQ(arr, expected) << name << " failed on random array";
}

TEST(SortingTest, MergeSort) {
    test_sorting([](std::vector<int>& v) { merge_sort(v); }, "MergeSort");
}

TEST(SortingTest, RadixSort) {
    test_sorting([](std::vector<int>& v) { radix_sort(v); }, "RadixSort");
}

TEST(SortingTest, QuickSort) {
    test_sorting([](std::vector<int>& v) { quick_sort(v); }, "QuickSort");
}