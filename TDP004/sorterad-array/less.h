#ifndef LESS_H
#define LESS_H

template <typename T>
class Less
{
public:
  bool compare(T const&, T const&) const;
};

#include "less.cc"
#endif
