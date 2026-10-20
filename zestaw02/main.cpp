#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "sortowanie.hpp"

template<typename Container>
void test_sort(const Container & data) {
    Container expected = data;
    std::sort(expected.begin(), expected.end());

    Container a = data;
    insertsort(a.begin(), a.end());
    assert(std::is_sorted(a.begin(), a.end()));
    assert(a == expected);

    Container b = data;
    quicksort(b.begin(), b.end());
    assert(std::is_sorted(b.begin(), b.end()));
    assert(b == expected);
}

std::vector<int> random_ints(int n, int max_value) {
    std::mt19937 gen(n);
    std::uniform_int_distribution<int> dist(0, max_value);
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        v[i] = dist(gen);
    }
    return v;
}

int main() {
    test_sort(std::vector<int>());
    test_sort(std::vector<int>{7});
    test_sort(std::vector<int>{2, 1});
    test_sort(std::vector<int>{9, 4, 7, 3, 6, 8, 2, 1, 5, 0});
    test_sort(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    test_sort(std::vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    test_sort(std::vector<int>(100, 5));
    test_sort(std::vector<int>{-3, 10, -3, 0, 7, -15, 10, 2});
    test_sort(std::vector<double>{3.5, -1.25, 2.0, 0.0, -7.5, 2.0});
    test_sort(std::vector<std::string>{"python", "c++", "aisd", "abc", "ab"});
    test_sort(std::array<int, 10>{{9, 4, 7, 3, 6, 8, 2, 1, 5, 0}});
    for (int n = 0; n <= 50; ++n) {
        test_sort(random_ints(n, 10));
    }
    test_sort(random_ints(2000, 1000000));

    int t[] = {9, 4, 7, 3, 6, 8, 2, 1, 5, 0};
    const int size = sizeof(t) / sizeof(*t);
    insertsort(t, t + size);
    assert(std::is_sorted(t, t + size));
    int u[] = {9, 4, 7, 3, 6, 8, 2, 1, 5, 0};
    quicksort(u, u + size);
    assert(std::is_sorted(u, u + size));

    std::vector<int> big = random_ints(1000000, 1000000);
    quicksort(big.begin(), big.end());
    assert(std::is_sorted(big.begin(), big.end()));

    std::cout << "Wszystkie testy zakończone pomyślnie." << std::endl;
    return 0;
}
