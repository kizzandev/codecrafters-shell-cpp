#include <iostream>
#include <vector>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> commands = {"echo", "type", "exit"};

  std::cout << "$ ";
  std::string input;
  while (std::getline(std::cin, input) && input.find("exit") != 0)
  {
    if (input.find("echo ") == 0)
      std::cout << input.substr(5) << '\n'
                << std::unitbuf;
    else if (input.find("type ") == 0)
    {
      std::string cmd = input.substr(5);
      bool found = false;
      for (auto &command : commands)
      {
        if (command == cmd)
        {
          std::cout << cmd << " is a shell builtin\n"
                    << std::unitbuf;
          found = true;
          break;
        }
      }
      if (!found)
      {
        std::cerr << cmd << ": not found\n"
                  << std::unitbuf;
      }
    }
    else
      std::cerr << input << ": command not found\n"
                << std::unitbuf;
    std::cout << "$ ";
  }
}
