#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  

  while (1) {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit 0") break;
    else if (input.substr(0,6) == "echo ") std::cout << input.substr(6) << std::unitbuf;
    else std::cerr << input << ": command not found\n" << std::unitbuf;
  }
}
