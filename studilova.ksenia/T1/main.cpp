#include <iostream>
#include <unordered_map>
#include <limits>

#include "commands.hpp"

int main()
{
  studilova::Context ctx;
  std::unordered_map< std::string, studilova::cmd_t > cmds;
  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, ctx);
    }
    catch(const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";

      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
