#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>
#include <cstddef>

namespace studilova
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct Triangle
  {
    Point a;
    Point b;
    Point c;
  };

  struct DelimiterIO
  {
    char exp;
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
  std::istream& operator>>(std::istream& in, Point& dest);
  std::istream& operator>>(std::istream& in, Polygon& dest);

  void readPoints(std::istream& in, std::vector< Point >& points, size_t count);
}

#endif