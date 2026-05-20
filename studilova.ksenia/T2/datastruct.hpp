#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <string>

namespace studilova
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
