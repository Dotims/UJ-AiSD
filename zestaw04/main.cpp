#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include "singlelist.hpp"

template <typename T>
std::string shown(const SingleList<T>& L) {
    std::ostringstream out;
    std::streambuf *old = std::cout.rdbuf(out.rdbuf());
    L.display();
    std::cout.rdbuf(old);
    return out.str();
}

template <typename T>
std::string shown_reversed(SingleList<T>& L) {
    std::ostringstream out;
    std::streambuf *old = std::cout.rdbuf(out.rdbuf());
    L.display_reversed();
    std::cout.rdbuf(old);
    return out.str();
}

void test_new_list() {
    SingleList<int> L;
    assert(L.empty());
    assert(!L.full());
    assert(L.size() == 0);
    assert(shown(L) == "\n");
}

void test_push_front_back() {
    SingleList<int> L;
    L.push_back(2);
    L.push_back(3);
    L.push_front(1);
    assert(shown(L) == "1 2 3 \n");
    assert(!L.empty());
    assert(L.size() == 3);
    assert(L.front() == 1);
    assert(L.back() == 3);

    L.front() = 10;
    L.back() = 30;
    assert(shown(L) == "10 2 30 \n");

    SingleList<int> M;
    M.push_front(5);
    assert(M.front() == 5);
    assert(M.back() == 5);
    M.push_back(6);
    assert(M.back() == 6);
}

void test_pop() {
    SingleList<int> L;
    for (int i = 1; i <= 4; ++i) {
        L.push_back(i);
    }
    L.pop_front();
    assert(shown(L) == "2 3 4 \n");
    L.pop_back();
    assert(shown(L) == "2 3 \n");
    assert(L.back() == 3);
    L.pop_back();
    assert(L.front() == 2);
    assert(L.back() == 2);
    L.pop_front();
    assert(L.empty());

    L.push_back(7);
    L.push_front(6);
    assert(shown(L) == "6 7 \n");
    L.pop_back();
    L.pop_back();
    L.push_front(8);
    assert(L.front() == 8);
    assert(L.back() == 8);
}

void test_clear() {
    SingleList<int> L;
    for (int i = 0; i < 5; ++i) {
        L.push_back(i);
    }
    L.clear();
    assert(L.empty());
    assert(L.size() == 0);
    L.clear();
    L.push_back(1);
    L.push_back(2);
    assert(shown(L) == "1 2 \n");
}

void test_copy() {
    SingleList<int> L;
    L.push_back(1);
    L.push_back(2);

    SingleList<int> K(L);
    K.push_back(3);
    K.front() = 100;
    assert(shown(L) == "1 2 \n");
    assert(shown(K) == "100 2 3 \n");

    SingleList<int> M;
    M.push_back(9);
    M.push_back(8);
    M.push_back(7);
    M = L;
    assert(shown(M) == "1 2 \n");
    M.pop_front();
    assert(shown(L) == "1 2 \n");
    M.push_back(5);
    assert(shown(M) == "2 5 \n");

    M = M;
    assert(shown(M) == "2 5 \n");

    SingleList<int> E;
    SingleList<int> F(E);
    assert(F.empty());
    F = L;
    assert(F.size() == 2);
    F = E;
    assert(F.empty());
}

void test_move() {
    SingleList<int> L;
    L.push_back(1);
    L.push_back(2);

    SingleList<int> K(std::move(L));
    assert(shown(K) == "1 2 \n");
    assert(L.empty());
    L.push_back(5);
    assert(shown(L) == "5 \n");

    SingleList<int> M;
    M.push_back(9);
    M = std::move(K);
    assert(shown(M) == "1 2 \n");
    assert(K.empty());
    M.push_back(3);
    assert(M.back() == 3);
}

void test_move_only_elements() {
    SingleList<std::unique_ptr<int> > L;
    L.push_back(std::unique_ptr<int>(new int(2)));
    L.push_front(std::unique_ptr<int>(new int(1)));
    L.insert(1, std::unique_ptr<int>(new int(5)));
    L.insert(3, std::unique_ptr<int>(new int(9)));
    L.insert(0, std::unique_ptr<int>(new int(0)));
    assert(L.size() == 5);
    assert(*L[0] == 0);
    assert(*L[1] == 1);
    assert(*L[2] == 5);
    assert(*L[3] == 2);
    assert(*L[4] == 9);
    assert(*L.back() == 9);
    L.erase(2);
    assert(L.size() == 4);
}

void test_indexes() {
    SingleList<int> L;
    L.push_back(10);
    L.push_back(30);
    L.insert(1, 20);
    L.insert(0, 0);
    L.insert(L.size(), 40);
    assert(shown(L) == "0 10 20 30 40 \n");
    assert(L.back() == 40);

    int item = 25;
    L.insert(3, item);
    assert(shown(L) == "0 10 20 25 30 40 \n");

    assert(L[0] == 0);
    assert(L[3] == 25);
    L[3] = 26;
    assert(L[3] == 26);
    const SingleList<int>& C = L;
    assert(C[5] == 40);

    assert(L.index(0) == 0);
    assert(L.index(30) == 4);
    assert(L.index(99) == -1);

    L.erase(0);
    L.erase(2);
    assert(shown(L) == "10 20 30 40 \n");
    L.erase(L.size() - 1);
    assert(shown(L) == "10 20 30 \n");
    assert(L.back() == 30);
    L.push_back(50);
    assert(shown(L) == "10 20 30 50 \n");
    L.erase(1);
    assert(shown(L) == "10 30 50 \n");

    int last = 60;
    L.insert(L.size(), last);
    assert(L.back() == 60);
    L.push_back(70);
    assert(shown(L) == "10 30 50 60 70 \n");

    SingleList<int> one;
    one.push_back(1);
    one.erase(0);
    assert(one.empty());
    one.push_back(2);
    assert(one.front() == 2);
    assert(one.back() == 2);
}

void test_reverse() {
    SingleList<int> E;
    E.reverse();
    assert(E.empty());

    SingleList<int> one;
    one.push_back(1);
    one.reverse();
    assert(shown(one) == "1 \n");

    SingleList<int> L;
    for (int i = 1; i <= 4; ++i) {
        L.push_back(i);
    }
    L.reverse();
    assert(shown(L) == "4 3 2 1 \n");
    assert(L.front() == 4);
    assert(L.back() == 1);
    L.push_back(0);
    assert(shown(L) == "4 3 2 1 0 \n");
    L.pop_back();
    L.pop_back();
    assert(shown(L) == "4 3 2 \n");
    L.reverse();
    assert(shown(L) == "2 3 4 \n");

    assert(shown_reversed(L) == "4 3 2 \n");
    assert(shown(L) == "2 3 4 \n");
    L.push_back(5);
    assert(shown(L) == "2 3 4 5 \n");
    assert(shown_reversed(E) == "\n");
}

void test_strings() {
    SingleList<std::string> L;
    std::string word = "ma";
    L.push_back(word);
    L.push_front(std::string("ala"));
    L.push_back("kota");
    assert(shown(L) == "ala ma kota \n");
    assert(word == "ma");
    assert(L.index("ma") == 1);
    assert(L.index("psa") == -1);
    L.insert(2, std::string("czarnego"));
    assert(shown(L) == "ala ma czarnego kota \n");
    assert(L[2] == "czarnego");
}

void test_many_elements() {
    SingleList<int> L;
    for (int i = 0; i < 100000; ++i) {
        L.push_back(i);
    }
    assert(L.size() == 100000);
    assert(L.front() == 0);
    assert(L.back() == 99999);
    L.reverse();
    assert(L.front() == 99999);
    assert(L.back() == 0);
    SingleList<int> K(L);
    assert(K.size() == 100000);
    L.clear();
    assert(L.empty());
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
    test_reverse();
    test_strings();
    test_many_elements();

    SingleList<int> L;
    for (int i = 1; i <= 5; ++i) {
        L.push_back(i);
    }
    L.display();
    L.display_reversed();

    std::cout << "Wszystkie testy zakończone pomyślnie." << std::endl;
    return 0;
}
