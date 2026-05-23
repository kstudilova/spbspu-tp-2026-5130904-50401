#ifndef CLI_HPP
#define CLI_HPP

#include <iosfwd>
#include <vector>

#include "polygon.hpp"

namespace stuilova{
  struct Command
  {
    static const std::vector< studilova::Polygon >* polygons;
  };

  std::istream& operator>>(std::istream& in, Command& command);
}

#endif
