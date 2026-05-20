#include <algorithm>
#include <iomanip>

#include "datastruct.hpp"

namespace
{
  std::string toBinary(unsigned long long value)
  {
    if (value == 0)
    {
      return "0";
    }

    std::string result;

    while (value != 0)
    {
      result += static_cast< char >('0' + value % 2);
      value /= 2;
    }

    std::reverse(result.begin(), result.end());
    return result;
  }

  bool markField(std::istream& in, bool& hasKey)
  {
    if (hasKey)
    {
      in.setstate(std::ios::failbit);
      return false;
    }

    hasKey = true;
    return true;
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

std::istream& studilova::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& studilova::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return in >> dest.ref;
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

  IOGuard guard(in);

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

std::istream& studilova::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input{};
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  if (!in)
  {
    return in;
  }

  while (in && in.peek() != ')')
  {
    std::string label;
    in >> LabelIO{ label };
    if (!in)
    {
      break;
    }

    if (label == "key1")
    {
      if (markField(in, hasKey1))
      {
        in >> ULLBinIO{ input.key1 };
      }
    }
    else if (label == "key2")
    {
      if (markField(in, hasKey2))
      {
        in >> ULLHexIO{ input.key2 };
      }
    }
    else if (label == "key3")
    {
      if (markField(in, hasKey3))
      {
        in >> StringIO{ input.key3 };
      }
    } else {
      in.setstate(std::ios::failbit);
      break;
    }

    in >> DelimiterIO{ ':' };
  }

  in >> DelimiterIO{ ')' };

  if (in && hasKey1 && hasKey2 && hasKey3)
  {
    dest = input;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& studilova::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  IOGuard guard(out);

  out << "(:key1 0b" << toBinary(dest.key1);
  out << ":key2 0x" << std::hex << std::uppercase << dest.key2;
  out << ":key3 \"" << dest.key3 << "\":)";

  return out;
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
