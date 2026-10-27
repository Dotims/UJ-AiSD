// arraylist.hpp

#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <algorithm>
#include <iostream>     // deklaracje strumieni cout, cin, cerr
#include <cassert>    // assert()
#include <utility>

template <typename T>
class ArrayList {
    T* tab;
    std::size_t msize; // najwieksza mozliwa liczba elementow
    std::size_t last; // pierwsza wolna pozycja
public:
    ArrayList(std::size_t s=10) : msize(s), last(0) {
        assert( s > 0 );
        tab = new T[s];
        assert( tab != nullptr );
    } // default constructor
    ~ArrayList() { delete [] tab; } // destruktor

    ArrayList(const ArrayList& other); // copy constructor
    // usage:   ArrayList<int> list2(list1);

    ArrayList(ArrayList&& other); // move constructor NIEOBOWIAZKOWE
    // usage:   ArrayList<int> list2(std::move(list1));

    ArrayList& operator=(const ArrayList& other); // copy assignment operator, return *this
    // usage:   list2 = list1;

    ArrayList& operator=(ArrayList&& other); // move assignment operator, return *this
    // usage:   list2 = std::move(list1); NIEOBOWIAZKOWE

    bool empty() const { return last == 0; } // checks if the container has no elements
    bool full() const { return last == msize; } // checks if the container is full
    std::size_t size() const { return last; } // liczba elementow na liscie
    std::size_t max_size() const { return msize; } // najwieksza mozliwa liczba elementow
    void push_front(const T& item); // dodanie na poczatek
    void push_front(T&& item); // dodanie na poczatek NIEOBOWIAZKOWE
    void push_back(const T& item); // dodanie na koniec
    void push_back(T&& item); // dodanie na koniec NIEOBOWIAZKOWE
    T& front(); // zwraca poczatek, nie usuwa, error dla pustej listy
    T& back(); // zwraca koniec, nie usuwa, error dla pustej listy
    void pop_front(); // usuwa poczatek, error dla pustej listy
    void pop_back(); // usuwa koniec, error dla pustej listy
    void clear(); // czyszczenie listy z elementow
    void display() const; // lepiej zdefiniowac operator<<
    void display_reversed() const;
    void reverse(); // odwracanie kolejnosci NIEOBOWIAZKOWE
    void sort(); // sortowanie listy NIEOBOWIAZKOWE
    void merge(ArrayList& other); //  merges two sorted lists into one NIEOBOWIAZKOWE

    // Operacje z indeksami. NIEOBOWIAZKOWE
    // https://en.cppreference.com/w/cpp/language/operators
    // Array subscript operator
    T& operator[](std::size_t pos); // podstawienie L[pos]=item, odczyt L[pos]
    const T& operator[](std::size_t pos) const; // dostep do obiektu const
    void erase(std::size_t pos); // usuniecie elementu na pozycji pos
    int index(const T& item); // jaki index na liscie (-1 gdy nie ma)
    void insert(std::size_t pos, const T& item); // inserts item before pos
    void insert(std::size_t pos, T&& item); // inserts item before pos
    // Jezeli pos=0, to wstawiamy na poczatek.
    // Jezeli pos=size(), to wstawiamy na koniec.

    friend std::ostream& operator<<(std::ostream& os, const ArrayList& L) {
        for (std::size_t i=0; i < L.last; ++i) { // odwolanie L.last
            os << L.tab[i] << " ";   // odwolanie L.tab
        }
        return os;
    } // usage:   std::cout << L << std::endl;
};

template <typename T>
ArrayList<T>::ArrayList(const ArrayList& other)
    : msize(other.msize), last(other.last) {
    tab = new T[msize];
    std::copy(other.tab, other.tab + other.last, tab);
}

template <typename T>
ArrayList<T>::ArrayList(ArrayList&& other)
    : tab(other.tab), msize(other.msize), last(other.last) {
    other.tab = nullptr;
    other.msize = 0;
    other.last = 0;
}

template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other) {
    if (this == &other) {
        return *this;
    }
    if (msize != other.msize) {
        T* copy = new T[other.msize];
        delete [] tab;
        tab = copy;
        msize = other.msize;
    }
    std::copy(other.tab, other.tab + other.last, tab);
    last = other.last;
    return *this;
}

template <typename T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList&& other) {
    std::swap(tab, other.tab);
    std::swap(msize, other.msize);
    std::swap(last, other.last);
    return *this;
}

template <typename T>
void ArrayList<T>::push_front(const T& item) {
    insert(0, item);
}

template <typename T>
void ArrayList<T>::push_front(T&& item) {
    insert(0, std::move(item));
}

template <typename T>
void ArrayList<T>::push_back(const T& item) {
    assert(!full());
    tab[last] = item;
    ++last;
}

template <typename T>
void ArrayList<T>::push_back(T&& item) {
    assert(!full());
    tab[last] = std::move(item);
    ++last;
}

template <typename T>
T& ArrayList<T>::front() {
    assert(!empty());
    return tab[0];
}

template <typename T>
T& ArrayList<T>::back() {
    assert(!empty());
    return tab[last - 1];
}

template <typename T>
void ArrayList<T>::pop_front() {
    erase(0);
}

template <typename T>
void ArrayList<T>::pop_back() {
    assert(!empty());
    --last;
}

template <typename T>
void ArrayList<T>::clear() {
    last = 0;
}

template <typename T>
void ArrayList<T>::display() const {
    std::cout << *this << std::endl;
}

template <typename T>
void ArrayList<T>::display_reversed() const {
    for (std::size_t i = last; i > 0; --i) {
        std::cout << tab[i - 1] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void ArrayList<T>::reverse() {
    std::reverse(tab, tab + last);
}

template <typename T>
void ArrayList<T>::sort() {
    std::sort(tab, tab + last);
}

template <typename T>
void ArrayList<T>::merge(ArrayList& other) {
    if (this == &other) {
        return;
    }
    assert(last + other.last <= msize);
    std::copy(other.tab, other.tab + other.last, tab + last);
    std::inplace_merge(tab, tab + last, tab + last + other.last);
    last += other.last;
    other.clear();
}

template <typename T>
T& ArrayList<T>::operator[](std::size_t pos) {
    assert(pos < last);
    return tab[pos];
}

template <typename T>
const T& ArrayList<T>::operator[](std::size_t pos) const {
    assert(pos < last);
    return tab[pos];
}

template <typename T>
void ArrayList<T>::erase(std::size_t pos) {
    assert(pos < last);
    std::copy(tab + pos + 1, tab + last, tab + pos);
    --last;
}

template <typename T>
int ArrayList<T>::index(const T& item) {
    for (std::size_t i = 0; i < last; ++i) {
        if (tab[i] == item) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

template <typename T>
void ArrayList<T>::insert(std::size_t pos, const T& item) {
    assert(pos <= last);
    assert(!full());
    std::copy_backward(tab + pos, tab + last, tab + last + 1);
    tab[pos] = item;
    ++last;
}

template <typename T>
void ArrayList<T>::insert(std::size_t pos, T&& item) {
    assert(pos <= last);
    assert(!full());
    std::copy_backward(tab + pos, tab + last, tab + last + 1);
    tab[pos] = std::move(item);
    ++last;
}

#endif

// EOF
