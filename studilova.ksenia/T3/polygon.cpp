#include "polygon.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <tuple>

namespace
{
  long long getOrientation(const studilova::Point& first, const studilova::Point& second, const studilova::Point& third)
  {
    long long x1 = second.x - first.x;
    long long y1 = second.y - first.y;
    long long x2 = third.x - second.x;
    long long y2 = third.y - second.y;

    return x1 * y2 - y1 * x1;
  }

  bool isBetween(int left, int value, int right)
  {
    return std::min(left, right) <= value && value <= std::max(left, right);
  }

  bool isPointOnSegment(const studilova::Point& point, const studilova::Segment& segment)
  {
    return getOrientation(segment.first, point, segment.second) == 0 &&
      isBetween(segment.first.x, point.x, segment.second.x) &&
      isBetween(segment.first.y, point.y, segment.second.y);
  }

  bool hasIntersectionWithSegment(const studilova::Segment& segment, const std::vector< studilova::Segment >& segments)
  {
    return std::any_of(segments.begin(), segments.end(),
      std::bind(studilova::segmentsIntersect, std::cref(segment), std::placeholders::_1));
  }

  bool hasIntersections(const std::vector< studilova::Segment >& lhs, const std::vector< studilova::Segment >& rhs)
  {
    return std::any_of(lhs.begin(), lhs.end(), std::bind(hasIntersectionWithSegment, std::placeholders::_1, std::cref(rhs)));
  }
}

studilova::IOGuard::IOGuard(std::basic_ios< char >& s) :
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  flags_(s.flags()),
  fill_(s.fill())
{}

studilova::IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.flags(flags_);
  s_.fill(fill_);
}

std::istream& studilova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  in >> c;

  if (in && c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& studilova::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point point{};
  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
  if (in)
  {
    dest = point;
  }
  return in;
}

bool studilova::operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

void studilova::readPoints(std::istream& in, std::vector< studilova::Point >& points, size_t count)
{
  if (count == 0)
  {
    return;
  }

  Point point{};
  if (!(in >> point))
  {
    in.setstate(std::ios::failbit);
    return;
  }

  points.push_back(point);
  readPoints(in, points, count - 1);
}

std::istream& studilova::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Polygon polygon{};
  size_t count = 0;
  in >> count;
  if (!in || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  polygon.points.reserve(count);
  readPoints(in, polygon.points, count);

  if (in && polygon.points.size() == count)
  {
    dest = polygon;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

studilova::Triangle studilova::makeTriangle(const std::vector< studilova::Point >& points, size_t index)
{
  return Triangle{ points[0], points[index], points[index + 1] };
}

double studilova::getTriangleArea(const studilova::Triangle& triangle)
{
  double first = static_cast< double >(triangle.a.x) * (triangle.b.y - triangle.c.y);
  double second = static_cast< double >(triangle.b.x) * (triangle.c.y - triangle.a.y);
  double third = static_cast< double >(triangle.c.x) * (triangle.a.y - triangle.b.y);

  return std::fabs(first + second + third) / 2.0;
}

double studilova::getArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    throw std::invalid_argument("invalid polygon");
  }

  std::vector< size_t > indexes(polygon.points.size() - 2);
  std::iota(indexes.begin(), indexes.end(), 1);

  std::vector< Triangle > triangles(indexes.size());
  std::transform(
    indexes.begin(),
    indexes.end(), triangles.begin(),
    std::bind(makeTriangle, std::cref(polygon.points), std::placeholders::_1)
  );

  std::vector< double > areas(triangles.size());
  std::transform(triangles.begin(), triangles.end(), areas.begin(), getTriangleArea);

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool studilova::hasEvenVertexes(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool studilova::hasOddVertexes(const Polygon& polygon)
{
  return polygon.points.size() % 2 != 0;
}

bool studilova::hasNVertexes(const Polygon& polygon, size_t count)
{
  return polygon.points.size() == count;
}

bool studilova::areaLess(const Polygon& lhs, const Polygon& rhs)
{
  return getArea(lhs) < getArea(rhs);
}

bool studilova::vertexesLess(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool studilova::isPermutation(const Polygon& polygon, const Polygon& ref)
{
  if (polygon.points.size() != ref.points.size())
  {
    return false;
  }
  return std::is_permutation(polygon.points.begin(), polygon.points.end(), ref.points.begin());
}

studilova::Segment studilova::makeSegment(const Point& first, const Point& second)
{
  return Segment{ first, second };
}

studilova::Segment studilova::makeSegmentByIndex(const Polygon& polygon, size_t index)
{
  size_t next = (index + 1) % polygon.points.size();
  return makeSegment(polygon.points[index], polygon.points[next]);
}

bool studilova::segmentsIntersect(const Segment& lhs, const Segment& rhs)
{
  long long first = getOrientation(lhs.first, lhs.second, rhs.first);
  long long second = getOrientation(lhs.first, lhs.second, rhs.second);
  long long third = getOrientation(rhs.first, rhs.second, lhs.first);
  long long fourth = getOrientation(rhs.first, rhs.second, lhs.second);

  if ((first > 0 && second < 0 || first < 0 && second > 0) && (third > 0 && fourth < 0 || third < 0 && fourth > 0))
  {
    return true;
  }

  return isPointOnSegment(rhs.first, lhs) || isPointOnSegment(rhs.second, lhs) ||
    isPointOnSegment(lhs.first, rhs) || isPointOnSegment(lhs.second, rhs);
}

bool studilova::polygonsIntersect(const Polygon& lhs, const Polygon& rhs)
{
  std::vector< size_t > lhsIndexes(lhs.points.size());
  std::iota(lhsIndexes.begin(), lhsIndexes.end(), 0);

  std::vector< size_t > rhsIndexes(rhs.points.size());
  std::iota(rhsIndexes.begin(), rhsIndexes.end(), 0);

  std::vector< Segment > lhsSegments(lhsIndexes.size());
  std::transform(lhsIndexes.begin(), lhsIndexes.end(), lhsSegments.begin(),
    std::bind(makeSegmentByIndex, std::cref(lhs), std::placeholders::_1));

  std::vector< Segment > rhsSegments(rhsIndexes.size());
  std::transform(rhsIndexes.begin(), rhsIndexes.end(), rhsSegments.begin(),
    std::bind(makeSegmentByIndex, std::cref(rhs), std::placeholders::_1));

  return hasIntersections(lhsSegments, rhsSegments);
}
