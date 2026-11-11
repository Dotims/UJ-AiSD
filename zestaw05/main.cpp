#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "doublelist.hpp"

template <typename T>
std::string shown(const DoubleList<T>& L) {
    std::ostringstream out;
    std::streambuf *old = std::cout.rdbuf(out.rdbuf());
    L.display();
    std::cout.rdbuf(old);
    return out.str();
}

template <typename T>
std::string shown_reversed(const DoubleList<T>& L) {
    std::ostringstream out;
    std::streambuf *old = std::cout.rdbuf(out.rdbuf());
    L.display_reversed();
    std::cout.rdbuf(old);
    return out.str();
}

void check(DoubleList<int>& L, const std::vector<int>& expected) {
    std::string forward, backward;
    for (std::size_t i = 0; i < expected.size(); ++i) {
        forward += std::to_string(expected[i]) + " ";
        backward = std::to_string(expected[i]) + " " + backward;
    }
    assert(shown(L) == forward + "\n");
    assert(shown_reversed(L) == backward + "\n");
    assert(L.size() == expected.size());
    assert(L.empty() == expected.empty());
    if (!expected.empty()) {
        assert(L.front() == expected.front());
        assert(L.back() == expected.back());
    }
}

void test_new_list() {
    DoubleList<int> L;
    assert(L.empty());
    assert(!L.full());
    assert(L.size() == 0);
    check(L, std::vector<int>());
    assert(shown(L) == "\n");
    assert(shown_reversed(L) == "\n");
}

void test_push_front_back() {
    DoubleList<int> L;
    L.push_back(2);
    check(L, std::vector<int>{2});
    L.push_back(3);
    L.push_front(1);
    check(L, std::vector<int>{1, 2, 3});

    L.front() = 10;
    L.back() = 30;
    check(L, std::vector<int>{10, 2, 30});

    DoubleList<int> M;
    M.push_front(5);
    check(M, std::vector<int>{5});
    M.push_front(4);
    M.push_back(6);
    check(M, std::vector<int>{4, 5, 6});

    int item = 7;
    M.push_back(item);
    M.push_front(item);
    check(M, std::vector<int>{7, 4, 5, 6, 7});
}

void test_pop() {
    DoubleList<int> L;
    for (int i = 1; i <= 5; ++i) {
        L.push_back(i);
    }
    L.pop_front();
    check(L, std::vector<int>{2, 3, 4, 5});
    L.pop_back();
    check(L, std::vector<int>{2, 3, 4});
    L.pop_back();
    L.pop_front();
    check(L, std::vector<int>{3});
    L.pop_back();
    check(L, std::vector<int>());

    L.push_back(7);
    L.push_front(6);
    check(L, std::vector<int>{6, 7});
    L.pop_front();
    L.pop_front();
    assert(L.empty());
    L.push_front(8);
    L.push_back(9);
    check(L, std::vector<int>{8, 9});
}

void test_clear() {
    DoubleList<int> L;
    for (int i = 0; i < 5; ++i) {
        L.push_back(i);
    }
    L.clear();
    check(L, std::vector<int>());
    L.clear();
    L.push_back(1);
    L.push_front(0);
    check(L, std::vector<int>{0, 1});
}

void test_copy() {
    DoubleList<int> L;
    L.push_back(1);
    L.push_back(2);

    DoubleList<int> K(L);
    K.push_back(3);
    K.front() = 100;
    check(L, std::vector<int>{1, 2});
    check(K, std::vector<int>{100, 2, 3});

    DoubleList<int> M;
    M.push_back(9);
    M.push_back(8);
    M.push_back(7);
    M = L;
    check(M, std::vector<int>{1, 2});
    M.pop_front();
    M.push_front(5);
    check(M, std::vector<int>{5, 2});
    check(L, std::vector<int>{1, 2});

    M = M;
    check(M, std::vector<int>{5, 2});

    DoubleList<int> E;
    DoubleList<int> F(E);
    check(F, std::vector<int>());
    F = L;
    check(F, std::vector<int>{1, 2});
    F = E;
    check(F, std::vector<int>());
}

void test_move() {
    DoubleList<int> L;
    L.push_back(1);
    L.push_back(2);

    DoubleList<int> K(std::move(L));
    check(K, std::vector<int>{1, 2});
    check(L, std::vector<int>());
    L.push_back(5);
    check(L, std::vector<int>{5});

    DoubleList<int> M;
    M.push_back(9);
    M = std::move(K);
    check(M, std::vector<int>{1, 2});
    check(K, std::vector<int>());
    M.push_back(3);
    M.push_front(0);
    check(M, std::vector<int>{0, 1, 2, 3});
}

void test_move_only_elements() {
    DoubleList<std::unique_ptr<int> > L;
    L.push_back(std::unique_ptr<int>(new int(1)));
    L.push_back(std::unique_ptr<int>(new int(2)));
    L.push_back(std::unique_ptr<int>(new int(3)));
    assert(L.size() == 3);
    assert(*L[0] == 1);
    assert(*L[1] == 2);
    assert(*L[2] == 3);
    assert(*L.front() == 1);
    assert(*L.back() == 3);
    L.erase(1);
    assert(L.size() == 2);
    assert(*L[1] == 3);
    L.pop_back();
    L.pop_front();
    assert(L.empty());
}

void test_indexes() {
    DoubleList<int> L;
    L.push_back(10);
    L.push_back(30);
    L.insert(1, 20);
    check(L, std::vector<int>{10, 20, 30});
    L.insert(0, 0);
    L.insert(L.size(), 40);
    check(L, std::vector<int>{0, 10, 20, 30, 40});

    int item = 25;
    L.insert(3, item);
    check(L, std::vector<int>{0, 10, 20, 25, 30, 40});
    int first = -1;
    L.insert(0, first);
    int last = 50;
    L.insert(L.size(), last);
    check(L, std::vector<int>{-1, 0, 10, 20, 25, 30, 40, 50});

    assert(L[0] == -1);
    assert(L[4] == 25);
    L[4] = 26;
    assert(L[4] == 26);
    const DoubleList<int>& C = L;
    assert(C[7] == 50);
    assert(C.index(30) == 5);
    assert(C.index(-1) == 0);
    assert(C.index(50) == 7);
    assert(C.index(99) == -1);

    L.erase(0);
    L.erase(3);
    check(L, std::vector<int>{0, 10, 20, 30, 40, 50});
    L.erase(L.size() - 1);
    check(L, std::vector<int>{0, 10, 20, 30, 40});
    L.push_back(60);
    check(L, std::vector<int>{0, 10, 20, 30, 40, 60});
    L.erase(2);
    check(L, std::vector<int>{0, 10, 30, 40, 60});
    L.push_front(-5);
    check(L, std::vector<int>{-5, 0, 10, 30, 40, 60});

    DoubleList<int> one;
    one.push_back(1);
    one.erase(0);
    check(one, std::vector<int>());
    one.push_back(2);
    check(one, std::vector<int>{2});
    one.insert(0, 1);
    one.insert(2, 3);
    check(one, std::vector<int>{1, 2, 3});
}

void test_strings() {
    DoubleList<std::string> L;
    std::string word = "ma";
    L.push_back(word);
    L.push_front(std::string("ala"));
    L.push_back("kota");
    assert(shown(L) == "ala ma kota \n");
    assert(shown_reversed(L) == "kota ma ala \n");
    assert(word == "ma");
    assert(L.index("ma") == 1);
    assert(L.index("psa") == -1);
    L.insert(2, std::string("czarnego"));
    assert(shown(L) == "ala ma czarnego kota \n");
    assert(shown_reversed(L) == "kota czarnego ma ala \n");
    assert(L[2] == "czarnego");
}

void test_many_elements() {
    DoubleList<int> L;
    for (int i = 0; i < 100000; ++i) {
        L.push_back(i);
    }
    assert(L.size() == 100000);
    assert(L.front() == 0);
    assert(L.back() == 99999);
    DoubleList<int> K(L);
    L.pop_front();
    L.pop_back();
    assert(L.size() == 99998);
    assert(K.size() == 100000);
    assert(K.back() == 99999);
    K.clear();
    assert(K.empty());
}

int main() {
    test_new_list();
    test_push_front_back();
    test_pop();
    test_clear();
    test_copy();
    test_move();
    test_move_only_elements();
    test_indexes();
    test_strings();
    test_many_elements();

    DoubleList<int> L;
    for (int i = 1; i <= 5; ++i) {
        L.push_back(i);
    }
    L.display();
    L.display_reversed();

    std::cout << "Wszystkie testy zakończone pomyślnie." << std::endl;
    return 0;
}
