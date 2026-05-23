#include "commands.hpp"

#include <algorithm>
#include <functional>
#include <iterator>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <string>

namespace
{
  bool isNumber(const std::string& str)
  {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
  }
}

void studilova::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }

  std::vector< Polygon > yapolygons;

  if (arg == "EVEN")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(yapolygons), hasEvenVertexes);
  }
  else if (arg == "ODD")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(yapolygons), hasOddVertexes);
  }
  else if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid command");
    }
    yapolygons = polygons;
  }
  else if (isNumber(arg))
  {
    size_t count = std::stoull(arg);
    if (count < 3)
    {
      throw std::invalid_argument("invalid command");
    }
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(yapolygons), std::bind(hasNVertexes, std::placeholders::_1, count));
  } else {
    throw std::invalid_argument("invalid command");
  }

  std::vector< double > areas(yapolygons.size());
  std::transform(yapolygons.begin(), yapolygons.end(), areas.begin(), getArea);
  double result = std::accumulate(areas.begin(), areas.end(), 0.0);

  if (arg == "MEAN")
  {
    result /= polygons.size();
  }

  out << std::fixed << std::setprecision(1) << result << '\n';
}

void studilova::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid command");
  }

  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }

  if (arg == "AREA")
  {
    auto result = std::max_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(*result) << '\n';
  }
  else if (arg == "VERTEXES")
  {
    auto result = std::max_element(polygons.begin(), polygons.end(), vertexesLess);
    out << result->points.size() << '\n';
  } else {
    throw std::invalid_argument("invalid command");
  }
}

void studilova::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid command");
  }

  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }

  if (arg == "AREA")
  {
    auto result = std::min_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(*result) << '\n';
  }
  else if (arg == "VERTEXES")
  {
    auto result = std::min_element(polygons.begin(), polygons.end(), vertexesLess);
    out << result->points.size() << '\n';
  } else {
    throw std::invalid_argument("invalid command");
  }
}

void studilova::count(std::istream& in, std::ostream& out, const std::vector< studilova::Polygon >& polygons)
{
  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }

  size_t result = 0;
  if (arg == "EVEN")
  {
    result = std::count_if(polygons.begin(), polygons.end(), hasEvenVertexes);
  }
  else if (arg == "ODD")
  {
    result = std::count_if(polygons.begin(), polygons.end(), hasOddVertexes);
  }
  else if (isNumber(arg))
  {
    size_t count = std::stoull(arg);
    if (count < 3)
    {
      throw std::invalid_argument("invalid command");
    }
    result = std::count_if(polygons.begin(), polygons.end(), std::bind(hasNVertexes, std::placeholders::_1, count));
  } else {
    throw std::invalid_argument("invalid command");
  }

  out << result << '\n';
}

void studilova::perms(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon ref{};
  if (!(in >> ref))
  {
    throw std::invalid_argument("invalid command");
  }

  auto predicate = std::bind(isPermutation, std::placeholders::_1, std::cref(ref));
  size_t result = std::count_if(polygons.begin(), polygons.end(), predicate);

  out << result << '\n';
}
