#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  

  std::cout << "$ ";
  std::string input;
  while (std::getline(std::cin, input) && input.find("exit") != 0) {
    if (input.find("echo ") == 0) std::cout << input.substr(5) << '\n' << std::unitbuf;
    else std::cerr << input << ": command not found\n" << std::unitbuf;
    std::cout << "$ ";
  }
}
