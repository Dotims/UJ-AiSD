// doublelist.hpp

#ifndef DOUBLELIST_HPP
#define DOUBLELIST_HPP

#include <iostream>     // deklaracje strumieni cout, cin, cerr
#include <cassert>    // assert()
#include <utility>

template <typename T>
struct DoubleNode {
// the default access mode and default inheritance mode are public
    T value;
    DoubleNode *next, *prev;
    // konstruktor domyslny (niepotrzebny)
    DoubleNode() : value(T()), next(nullptr), prev(nullptr) {}
    DoubleNode(const T& item, DoubleNode *nptr=nullptr, DoubleNode *pptr=nullptr)
        : value(item), next(nptr), prev(pptr) {} // konstruktor
    ~DoubleNode() = default; // destruktor
};


template <typename T>
class DoubleList {
    DoubleNode<T> *head, *tail;
public:
    DoubleList() : head(nullptr), tail(nullptr) {}
    ~DoubleList() { clear(); } // tu trzeba wyczyscic wezly

    DoubleList(const DoubleList& other); // copy constructor
    // usage:   DoubleList<int> list2(list1);

    DoubleList(DoubleList&& other); // move constructor NIEOBOWIAZKOWE
    // usage:   DoubleList<int> list2(std::move(list1));

    DoubleList& operator=(const DoubleList& other); // copy assignment operator, return *this
    // usage:   list2 = list1;

    DoubleList& operator=(DoubleList&& other); // move assignment operator, return *this
    // usage:   list2 = std::move(list1); NIEOBOWIAZKOWE

    bool empty() const { return head == nullptr; }
    bool full() const { return false; }
    std::size_t size() const; // O(n) bo trzeba policzyc
    void push_front(const T& item); // O(1)
    void push_front(T&& item); // O(1) NIEOBOWIAZKOWE
    void push_back(const T& item); // O(1)
    void push_back(T&& item); // O(1) NIEOBOWIAZKOWE
    T& front() { assert(!empty()); return head->value; } // zwraca poczatek, nie usuwa
    T& back() { assert(!empty()); return tail->value; } // zwraca koniec, nie usuwa
    void pop_front(); // usuwa poczatek O(1)
    void pop_back(); // usuwa koniec O(1)
    void clear(); // czyszczenie listy z elementow O(n)
    void display() const; // O(n)
    void display_reversed() const; // O(n)

    // Operacje z indeksami. NIEOBOWIAZKOWE
    T& operator[](std::size_t pos); // podstawienie L[pos]=item, odczyt L[pos]
    const T& operator[](std::size_t pos) const; // dostep do obiektu const
    void erase(std::size_t pos);
    int index(const T& item) const; // jaki index na liscie (-1 gdy nie ma) O(n)
    void insert(std::size_t pos, const T& item); // inserts item before pos,
    void insert(std::size_t pos, T&& item); // inserts item before pos,
    // Jezeli pos=0, to wstawiamy na poczatek.
    // Jezeli pos=size(), to wstawiamy na koniec.
};

template <typename T>
void DoubleList<T>::push_front(const T& item) {
    if (!empty()) {
        head = new DoubleNode<T>(item, head);
        head->next->prev = head;
    } else {
        head = tail = new DoubleNode<T>(item);
    }
}

template <typename T>
void DoubleList<T>::push_front(T&& item) {
    if (!empty()) {
        head = new DoubleNode<T>(std::move(item), head);
        head->next->prev = head;
    } else {
        head = tail = new DoubleNode<T>(std::move(item));
    }
}

template <typename T>
DoubleList<T>::DoubleList(const DoubleList& other) : head(nullptr), tail(nullptr) {
    for (DoubleNode<T> *node = other.head; node != nullptr; node = node->next) {
        push_back(node->value);
    }
}

template <typename T>
DoubleList<T>::DoubleList(DoubleList&& other) : head(other.head), tail(other.tail) {
    other.head = other.tail = nullptr;
}

template <typename T>
DoubleList<T>& DoubleList<T>::operator=(const DoubleList& other) {
    if (this != &other) {
        clear();
        for (DoubleNode<T> *node = other.head; node != nullptr; node = node->next) {
            push_back(node->value);
        }
    }
    return *this;
}

template <typename T>
DoubleList<T>& DoubleList<T>::operator=(DoubleList&& other) {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        other.head = other.tail = nullptr;
    }
    return *this;
}

template <typename T>
std::size_t DoubleList<T>::size() const {
    std::size_t count = 0;
    for (DoubleNode<T> *node = head; node != nullptr; node = node->next) {
        ++count;
    }
    return count;
}

template <typename T>
void DoubleList<T>::push_back(const T& item) {
    if (!empty()) {
        tail->next = new DoubleNode<T>(item, nullptr, tail);
        tail = tail->next;
    } else {
        head = tail = new DoubleNode<T>(item);
    }
}

template <typename T>
void DoubleList<T>::push_back(T&& item) {
    DoubleNode<T> *node = new DoubleNode<T>();
    node->value = std::move(item);
    node->prev = tail;
    if (empty()) {
        head = node;
    } else {
        tail->next = node;
    }
    tail = node;
}

template <typename T>
void DoubleList<T>::pop_front() {
    assert(!empty());
    DoubleNode<T> *node = head;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete node;
}

template <typename T>
void DoubleList<T>::pop_back() {
    assert(!empty());
    DoubleNode<T> *node = tail;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete node;
}

template <typename T>
void DoubleList<T>::clear() {
    while (!empty()) {
        pop_front();
    }
}

template <typename T>
void DoubleList<T>::display() const {
    for (DoubleNode<T> *node = head; node != nullptr; node = node->next) {
        std::cout << node->value << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void DoubleList<T>::display_reversed() const {
    for (DoubleNode<T> *node = tail; node != nullptr; node = node->prev) {
        std::cout << node->value << " ";
    }
    std::cout << std::endl;
}

template <typename T>
T& DoubleList<T>::operator[](std::size_t pos) {
    assert(pos < size());
    DoubleNode<T> *node = head;
    for (std::size_t i = 0; i < pos; ++i) {
        node = node->next;
    }
    return node->value;
}

template <typename T>
const T& DoubleList<T>::operator[](std::size_t pos) const {
    assert(pos < size());
    DoubleNode<T> *node = head;
    for (std::size_t i = 0; i < pos; ++i) {
        node = node->next;
    }
    return node->value;
}

template <typename T>
void DoubleList<T>::erase(std::size_t pos) {
    assert(pos < size());
    if (pos == 0) {
        pop_front();
        return;
    }
    DoubleNode<T> *node = head;
    for (std::size_t i = 0; i < pos; ++i) {
        node = node->next;
    }
    if (node == tail) {
        pop_back();
        return;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
}

template <typename T>
int DoubleList<T>::index(const T& item) const {
    int pos = 0;
    for (DoubleNode<T> *node = head; node != nullptr; node = node->next) {
        if (node->value == item) {
            return pos;
        }
        ++pos;
    }
    return -1;
}

template <typename T>
void DoubleList<T>::insert(std::size_t pos, const T& item) {
    assert(pos <= size());
    if (pos == 0) {
        push_front(item);
        return;
    }
    DoubleNode<T> *after = head;
    for (std::size_t i = 0; i < pos; ++i) {
        after = after->next;
    }
    if (after == nullptr) {
        push_back(item);
        return;
    }
    DoubleNode<T> *node = new DoubleNode<T>(item, after, after->prev);
    after->prev->next = node;
    after->prev = node;
}

template <typename T>
void DoubleList<T>::insert(std::size_t pos, T&& item) {
    assert(pos <= size());
    if (pos == 0) {
        push_front(std::move(item));
        return;
    }
    DoubleNode<T> *after = head;
    for (std::size_t i = 0; i < pos; ++i) {
        after = after->next;
    }
    if (after == nullptr) {
        push_back(std::move(item));
        return;
    }
    DoubleNode<T> *node = new DoubleNode<T>();
    node->value = std::move(item);
    node->next = after;
    node->prev = after->prev;
    after->prev->next = node;
    after->prev = node;
}

#endif

// EOF
