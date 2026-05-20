#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "datastruct.hpp"

int main()
{
  using data_t = studilova::DataStruct;
  std::vector< data_t > data;

  using input_it_t = std::istream_iterator< data_t >;

  while (!std::cin.eof())
  {
    std::copy(input_it_t{ std::cin }, input_it_t{}, std::back_inserter(data));

    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());

  using output_it_t = std::ostream_iterator< data_t >;
  std::copy(data.begin(), data.end(), output_it_t{ std::cout, "\n" });

  return 0;
}
