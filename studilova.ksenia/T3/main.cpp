#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "cli.hpp"
#include "polygon.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "wrong arguments" << '\n';
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "cannot open file" << '\n';
    return 1;
  }

  std::vector< studilova::Polygon > polygons;
  studilova::readData(input, polygons);

  studilova::Command::polygons = &polygons;
  std::vector< studilova::Command > commands;

  std::copy(std::istream_iterator< studilova::Command >{std::cin}, std::istream_iterator< studilova::Command >{}, std::back_inserter(commands));

  return 0;
}
