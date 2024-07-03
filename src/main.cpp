#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  

  while (1) {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    std::cerr << input << ": command not found" << std::unitbuf;
  }
}
