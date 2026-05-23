#include "cli.hpp"
#include "commands.hpp"

#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

const std::vector< studilova::Polygon >* studilova::Command::polygons = nullptr;

std::istream& studilova::operator>>(std::istream& in, Command&)
{
  std::string name;
  in >> name;
  if (!in)
  {
    return in;
  }

  using command_t = std::function< void(std::istream&, std::ostream&, const std::vector< Polygon >&) >;

  static std::unordered_map< std::string, command_t > commands;

  commands["AREA"] = area;
  commands["MAX"] = max;
  commands["MIN"] = min;
  commands["COUNT"] = count;
  commands["PERMS"] = perms;
  commands["INTERSECTIONS"] = intersections;

  try
  {
    commands.at(name)(in, std::cout, *Command::polygons);
  }
  catch(...)
  {
    std::cout << "<INVALID COMMAND>" << '\n';
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return in;
}
