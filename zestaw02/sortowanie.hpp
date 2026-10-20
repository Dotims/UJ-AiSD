#ifndef SORTOWANIE_HPP
#define SORTOWANIE_HPP

#include <iterator>
#include <utility>

template<typename Iterator>
void insertsort(Iterator first, Iterator last) {
    typedef typename std::iterator_traits<Iterator>::value_type Value;
    if (first == last) {
        return;
    }
    for (Iterator i = std::next(first); i != last; ++i) {
        Value item = std::move(*i);
        Iterator j = i;
        while (j != first) {
            Iterator prev = std::prev(j);
            if (!(item < *prev)) {
                break;
            }
            *j = std::move(*prev);
            j = prev;
        }
        *j = std::move(item);
    }
}

template<typename Iterator>
void quicksort(Iterator first, Iterator last) {
    typedef typename std::iterator_traits<Iterator>::difference_type Index;
    typedef typename std::iterator_traits<Iterator>::value_type Value;
    Index right = (last - first) - 1;
    if (right < 1) {
        return;
    }
    Value pivot = first[right / 2];
    Index i = 0;
    Index j = right;
    while (i <= j) {
        while (first[i] < pivot) {
            ++i;
        }
        while (pivot < first[j]) {
            --j;
        }
        if (i <= j) {
            std::swap(first[i], first[j]);
            ++i;
            --j;
        }
    }
    quicksort(first, first + (j + 1));
    quicksort(first + i, last);
}

#endif
