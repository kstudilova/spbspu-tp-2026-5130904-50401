#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>

namespace studilova
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
