#include <utility>
#include <stdexcept>

using namespace std;

template <typename T, typename C>
Sorted_Array<T, C>::Sorted_Array(initializer_list<T> list)
    : data{}, comparator{}
{
    for (T const& val : list)
    {
        insert(val);
    }
}

template <typename T, typename C>
Sorted_Array<T, C>::Sorted_Array(initializer_list<T> list, C const& comparator)
 : data{}, comparator{comparator}
 {
   for (T const& val : list)
   {
       insert(val);
   }
 }

template <typename T, typename C>
Sorted_Array<T, C>& Sorted_Array<T, C>::operator=(initializer_list<T> list)
{
    Sorted_Array tmp{list};
    swap(data, tmp.data);
    return *this;
}

template <typename T, typename C>
void Sorted_Array<T, C>::insert(T const& value)
{
  data.push_back(value);
  for (int i {size() - 1}; i > 0; --i)
  {
    if (comparator.compare(data[i], data[i-1]))
    {
        swap(data[i-1], data[i]);
    }
    else
    {
        break;
    }
  }
}

template <typename T, typename C>
T Sorted_Array<T, C>::erase(int index)
{
    if (index < 0 || index >= size())
        throw std::out_of_range{"unable to erase; index is out of bounds."};
    for (int i {index}; i + 1 < size(); ++i)
    {
        swap(data[i], data[i+1]);
    }
    T value {data.back()};
    data.pop_back();
    return value;
}

template <typename T, typename C>
T & Sorted_Array<T, C>::operator[](int index)
{
    return data[index];
}

template <typename T, typename C>
T Sorted_Array<T, C>::operator[](int index) const
{
    return data[index];
}

template <typename T, typename C>
T & Sorted_Array<T, C>::at(int index)
{
    return data.at(index);
}

template <typename T, typename C>
T Sorted_Array<T, C>::at(int index) const
{
    return data.at(index);
}

template <typename T, typename C>
int Sorted_Array<T, C>::size() const
{
    return static_cast<int>(data.size());
}

template <typename T>
ostream & operator<<(ostream & os, Sorted_Array<T> const & array)
{
    if (array.size() == 0)
    {
        os << "{}";
        return os;
    }
    os << '{' << array[0];
    for (int i {1}; i < array.size(); ++i)
    {
        os << ", " << array[i];
    }
    os << '}';
    return os;
}
