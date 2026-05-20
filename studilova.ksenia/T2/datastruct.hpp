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
    std::string& ref;
  };

  struct ULLBinIO
  {
    unsigned long long& ref;
  };

  struct ULLHexIO
  {
    unsigned long long& ref;
  };

  class IOGuard
  {
    public:
      explicit IOGuard(std::basic_ios< char >& s);
      ~IOGuard();

    private:
      std::basic_ios< char >& s_;
      std::streamsize width_;
      std::streamsize precision_;
      std::basic_ios< char >::fmtflags flags_;
      char fill_;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, ULLBinIO&& dest);
  std::istream& operator>>(std::istream& in, ULLHexIO&& dest);

  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
