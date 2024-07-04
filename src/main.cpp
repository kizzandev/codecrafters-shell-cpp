#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

enum Commands {
  cmd_echo,
  cmd_type,
  cmd_exit,
  cmd_notValid,
  cmd_ls,
  cmd_abcd,
  cmd_pwd,
  cmd_cd
};

// . // .. // path/to //~/home
enum Directions { dir_current, dir_prev, dir_to, dir_home };

Commands strToCmd(const std::string &cmd) {
  if (cmd.find("echo") == 0)
    return cmd_echo;
  else if (cmd.find("type") == 0)
    return cmd_type;
  else if (cmd.find("exit") == 0)
    return cmd_exit;
  else if (cmd.find("ls") == 0)
    return cmd_ls;
  else if (cmd.find("abcd") == 0)
    return cmd_abcd;
  else if (cmd.find("pwd") == 0)
    return cmd_pwd;
  else if (cmd.find("cd") == 0)
    return cmd_cd;
  else
    return cmd_notValid;
}

std::string get_path(const std::string &command) {
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

void execute_ext_command(const std::string &command,
                         const std::vector<std::string> &args) {
  std::string path = get_path(command);
  if (path.empty()) {
    std::cerr << command << ": command not found\n";
    return;
  }

  std::vector<std::unique_ptr<char[]>> c_args;
  c_args.push_back(std::make_unique<char[]>(path.size() + 1));
  std::strcpy(c_args.back().get(), path.c_str());

  for (const auto &arg : args) {
    c_args.push_back(std::make_unique<char[]>(arg.size() + 1));
    std::strcpy(c_args.back().get(), arg.c_str());
  }
  c_args.push_back(nullptr);

  std::vector<char *> c_args_raw;
  for (const auto &arg : c_args) {
    c_args_raw.push_back(arg.get());
  }

  pid_t pid = fork();
  if (pid == 0) {
    execvp(c_args_raw[0], c_args_raw.data());
    std::cerr << "Failed to execute " << command << '\n';
    exit(EXIT_FAILURE);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    std::cerr << "Failed to fork\n";
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  bool exit = false;

  std::cout << "$ ";
  std::string input;
  while (!exit && std::getline(std::cin, input)) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
      args.push_back(arg);
    }

    Commands cmd = strToCmd(input);
    switch (cmd) {
      case cmd_echo: {
        for (size_t i = 0; i < args.size(); ++i) {
          std::cout << args[i];
          if (i != args.size() - 1) std::cout << ' ';
        }
        std::cout << '\n';
        break;
      }
      case cmd_type: {
        if (args.empty()) {
          std::cerr << "type: missing argument\n";
          break;
        }

        std::string cmd = args[0];
        Commands builtin = strToCmd(cmd);
        if (builtin != cmd_notValid) {
          std::cout << cmd << " is a shell builtin\n";
        } else {
          std::string path = get_path(cmd);
          if (path.empty()) {
            std::cerr << cmd << ": not found\n";
          } else {
            std::cout << cmd << " is " << path << '\n';
          }
        }
        break;
      }
      case cmd_exit: {
        exit = true;
        break;
      }
      case cmd_pwd: {
        std::string path = std::filesystem::current_path();
        std::cout << path << '\n';
        break;
      }
      case cmd_cd: {
        if (args.empty()) {
          std::cerr << "cd: missing argument\n";
          break;
        }

        std::string path = args[0];
        if (path == "..") {
          std::filesystem::current_path(std::filesystem::current_path().parent_path());
        } else if (path == "~") {
          std::filesystem::current_path(std::filesystem::path(std::getenv("HOME")));
        } else if (path == ".") {
          std::filesystem::current_path(std::filesystem::current_path());
        } else {
          std::filesystem::current_path(path);
        }

        std::cout << "cd: " << std::filesystem::current_path() << '\n';
        break;
      }
      default:
        execute_ext_command(command, args);
        break;
    }
    if (!exit) std::cout << "$ ";
  }
}
