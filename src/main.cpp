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

    if (input.substr(0,5) == "echo") std::cout << input.substr(6,input.size()-1) << '\n' << std::unitbuf;
    else std::cerr << input << ": command not found\n" << std::unitbuf;
  }
}
