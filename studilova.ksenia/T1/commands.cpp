#include <stdexcept>

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
