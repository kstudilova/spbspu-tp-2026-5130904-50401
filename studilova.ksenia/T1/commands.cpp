#include <stdexcept>
#include <iomanip>

#include "commands.hpp"

void studilova::note(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if (!in || ctx.notes.count(name))
  {
    throw std::logic_error("Invalid");
  }

  auto n = std::make_shared< Note >();
  n->name = name;

  ctx.notes[name] = n;
}

void studilova::line(std::istream& in, std::ostream&, Context& ctx)
{
  std::string name;
  std::string text;

  in >> name >> std::quoted(text);

  if(!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if (it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }
  it->second->lines.push_back(text);
}

void studilova::show(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if (!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if (it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }

  const auto& lines = it->second->lines;
  for (size_t i = 0; i < lines.size(); ++i)
  {
    out << lines[i] << "\n";
  }
}

void studilova::drop(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if (!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if (it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }
  ctx.notes.erase(it);
}
