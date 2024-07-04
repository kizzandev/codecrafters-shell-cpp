#include <iostream>

enum Commands { cmd_echo, cmd_type, cmd_exit, cmd_notValid };

Commands strToCmd(std::string cmd) {
  if (cmd.find("echo ") == 0)
    return cmd_echo;
  else if (cmd.find("type ") == 0)
    return cmd_type;
  else if (cmd.find("exit ") == 0)
    return cmd_exit;
  else
    return cmd_notValid;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  bool exit = false;

  std::cout << "$ ";
  std::string input;
  while (std::getline(std::cin, input) && !exit) {
    Commands cmd = strToCmd(input);

    switch (cmd) {
      case cmd_echo:
        std::cout << input.substr(5) << '\n' << std::unitbuf;
        break;
      case cmd_type: {
        std::string cmd = input.substr(5);
        Commands builtin = strToCmd(cmd);
        if (builtin != cmd_notValid) {
          std::cout << cmd << " is a shell builtin\n" << std::unitbuf;
        } else {
          std::cerr << cmd << ": not found\n" << std::unitbuf;
        }
        break;
      }
      case cmd_exit:
        exit = true;
        break;
      default:
        std::cerr << input << ": command not found\n" << std::unitbuf;
        break;
    }
    std::cout << "$ ";
  }
}
