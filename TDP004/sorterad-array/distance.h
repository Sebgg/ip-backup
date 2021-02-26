#ifndef DISTANCE_H
#define DISTANCE_H

template <typename T>
class Distance
{
public:
  Distance(T);
  bool compare(T const&, T const&) const;
private:
  T center{};
};

#include "distance.tcc"
#endif
