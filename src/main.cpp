#include <filesystem>
#include <iostream>
#include <sstream>

enum Commands { cmd_echo, cmd_type, cmd_exit, cmd_notValid, cmd_ls, cmd_abcd };

Commands strToCmd(std::string cmd) {
  if (cmd.find("echo") == 0)
    return cmd_echo;
  else if (cmd.find("type") == 0)
    return cmd_type;
  else if (cmd.find("exit 0") == 0)
    return cmd_exit;
  else if (cmd.find("ls") == 0)
    return cmd_ls;
  else if (cmd.find("abcd") == 0)
    return cmd_abcd;
  else
    return cmd_notValid;
}

std::string get_path(std::string command) {
  std::string env_path = std::getenv("PATH");
  std::stringstream ss(env_path);
  std::string path;

  while (!ss.eof()) {
    getline(ss, path, ':');
    std::string abs_path = path + '/' + command;
    if (std::filesystem::exists(abs_path)) {
      return abs_path;
    }
  }
  return "";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  bool exit = false;

  std::cout << "$ ";
  std::string input;
  while (!exit && std::getline(std::cin, input)) {
    Commands cmd = strToCmd(input);

    switch (cmd) {
      case cmd_echo:
        std::cout << input.substr(5) << '\n';
        break;
      case cmd_type: {
        std::string cmd = input.substr(5);
        Commands builtin = strToCmd(cmd);
        if (builtin != cmd_notValid) {
          std::cout << cmd << " is a shell builtin\n";
        } else {
          std::string path = get_path(cmd);

          if (path.empty()) {
            std::cerr << cmd << ": not found\n";
          } else {
            std::cout << cmd << " is " << path << std::endl;
          }
        }
        break;
      }
      case cmd_exit: {
        exit = true;
        break;
      }
      default:
        std::cerr << input << ": command not found\n";
        break;
    }
    std::cout << "$ ";
  }
}
