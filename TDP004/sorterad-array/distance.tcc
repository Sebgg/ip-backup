#include <cmath>

template <typename T>
Distance<T>::Distance(T center)
 : center{center}
{
}

template <typename T>
bool Distance<T>::compare(T const& left, T const& right) const
{
  return fabs(left - center) < fabs(right - center);
}
