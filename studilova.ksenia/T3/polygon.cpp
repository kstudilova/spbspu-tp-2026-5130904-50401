#include "polygon.hpp"

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