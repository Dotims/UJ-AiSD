#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "arraylist.hpp"

std::string as_string(const ArrayList<int>& L) {
    std::ostringstream out;
    out << L;
    return out.str();
}

void test_new_list() {
    ArrayList<int> L(5);
    assert(L.empty());
    assert(!L.full());
    assert(L.size() == 0);
    assert(L.max_size() == 5);

    ArrayList<int> D;
    assert(D.max_size() == 10);
}

void test_push_and_pop() {
    ArrayList<int> L(4);
    L.push_back(2);
    L.push_back(3);
    L.push_front(1);
    assert(as_string(L) == "1 2 3 ");
    assert(L.size() == 3);
    assert(L.front() == 1);
    assert(L.back() == 3);

    L.push_front(0);
    assert(L.full());

    L.front() = 10;
    L.back() = 30;
    assert(as_string(L) == "10 1 2 30 ");

    L.pop_front();
    assert(as_string(L) == "1 2 30 ");
    L.pop_back();
    assert(as_string(L) == "1 2 ");
    L.pop_back();
    L.pop_back();
    assert(L.empty());

    L.push_back(7);
    L.clear();
    assert(L.empty());
    assert(L.max_size() == 4);
}

void test_copy() {
    ArrayList<int> L(5);
    L.push_back(1);
    L.push_back(2);

    ArrayList<int> K(L);
    K.push_back(3);
    assert(as_string(L) == "1 2 ");
    assert(as_string(K) == "1 2 3 ");

    ArrayList<int> M(2);
    M = K;
    assert(M.max_size() == 5);
    assert(as_string(M) == "1 2 3 ");
    M.pop_front();
    assert(as_string(K) == "1 2 3 ");

    M = M;
    assert(as_string(M) == "2 3 ");
}

void test_move() {
    ArrayList<int> L(3);
    L.push_back(1);
    L.push_back(2);

    ArrayList<int> K(std::move(L));
    assert(as_string(K) == "1 2 ");
    assert(L.empty());

    ArrayList<int> M(6);
    M.push_back(9);
    M = std::move(K);
    assert(as_string(M) == "1 2 ");
    assert(M.max_size() == 3);
}

void test_indexes() {
    ArrayList<int> L(6);
    L.push_back(10);
    L.push_back(30);
    L.insert(1, 20);
    L.insert(0, 0);
    L.insert(L.size(), 40);
    assert(as_string(L) == "0 10 20 30 40 ");

    assert(L[2] == 20);
    L[2] = 25;
    const ArrayList<int>& C = L;
    assert(C[2] == 25);

    assert(L.index(30) == 3);
    assert(L.index(99) == -1);

    L.erase(0);
    L.erase(1);
    L.erase(L.size() - 1);
    assert(as_string(L) == "10 30 ");
}

void test_reverse_sort_merge() {
    ArrayList<int> L(8);
    L.push_back(5);
    L.push_back(1);
    L.push_back(4);
    L.reverse();
    assert(as_string(L) == "4 1 5 ");
    L.sort();
    assert(as_string(L) == "1 4 5 ");

    ArrayList<int> K(3);
    K.push_back(2);
    K.push_back(3);
    K.push_back(6);
    L.merge(K);
    assert(as_string(L) == "1 2 3 4 5 6 ");
    assert(K.empty());

    ArrayList<int> E(2);
    E.reverse();
    E.sort();
    assert(E.empty());
}

void test_strings() {
    ArrayList<std::string> L(3);
    std::string word = "ma";
    L.push_back(word);
    L.push_front(std::string("ala"));
    L.push_back("kota");
    assert(L.full());
    assert(L.front() == "ala");
    assert(L.back() == "kota");
    assert(L.index("ma") == 1);
    L.sort();
    assert(L[0] == "ala");
    assert(L[1] == "kota");
}

int main() {
    test_new_list();
    test_push_and_pop();
    test_copy();
    test_move();
    test_indexes();
    test_reverse_sort_merge();
    test_strings();

    ArrayList<int> L(5);
    for (int i = 1; i <= 5; ++i) {
        L.push_back(i);
    }
    L.display();
    L.display_reversed();

    std::cout << "Wszystkie testy zakończone pomyślnie." << std::endl;
    return 0;
}
