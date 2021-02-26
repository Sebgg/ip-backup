#ifndef SORTED_ARRAY_H
#define SORTED_ARRAY_H

#include <vector>
#include <initializer_list>
#include <ostream>
#include "less.h"
/**
* Vi förstår inte riktigt om du menar att vi ska inkludera distance.h här
* så vi gör det. Men det känns överflödigt, och programmet kompilerar utan den.
*/
/* Helt korrekt här skall inte distance.h inkluderas, det var jag som skrev
 * det på fel plats. Det var menat för sorted_array_test.cc, ber om ursält för
 * missförståndet.
 */
#include "distance.h"

template <typename T=int, typename C=Less<T>>
class Sorted_Array
{

public:

    Sorted_Array() = default;
    Sorted_Array(std::initializer_list<T> list);
    Sorted_Array(std::initializer_list<T> list, C const& comparator);

    Sorted_Array& operator=(std::initializer_list<T> list);

    void insert(T const& value);
    T erase(int index);

    T & operator[](int index);
    T operator[](int index) const;

    T & at(int index);
    T at(int index) const;

    int size() const;

private:

    std::vector<T> data{};
    C comparator{};
};

template<typename T=int, typename C=Less<T>>
std::ostream & operator<<(std::ostream & os, Sorted_Array<T, C> const & array);

#include "sorted_array.cc"
#endif//SORTED_ARRAY_H
