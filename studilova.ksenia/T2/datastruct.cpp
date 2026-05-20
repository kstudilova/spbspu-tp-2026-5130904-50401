#include "datastruct.hpp"

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

std::istream& studilova::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline( in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& studilova::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string data = "";
  if ((in >> StringIO{ data }) && (data != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& studilova::operator>>(std::istream& in, ULLBinIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char zero = '0';
  char b = '0';
  in >> zero >> b;

  if (!in)
  {
    return in;
  }

  if (zero != '0' || (b != 'b' && b != 'B'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long result = 0;
  bool hasDigits = false;

  while (in)
  {
    char c = in.peek();
    if (c != '0' && c != '1')
    {
      break;
    }
    in.get(c);

    result *= 2;
    result += (c - '0');
    hasDigits = true;
  }

  if (!hasDigits)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = result;
  return in;
}

std::istream& studilova::operator>>(std::istream& in, ULLHexIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char zero = '0';
  char x = '0';
  in >> zero >> x;

  if (!in)
  {
    return in;
  }

  if (zero != '0' || (x != 'x' && x != 'X'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  in >> std::hex >> dest.ref;
  return in;
}

bool studilova::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}
