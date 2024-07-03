#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  

  std::cout << "$ ";
  std::string input;
  while (input.find("exit") != 0) {
    std::getline(std::cin, input);

    if (input.find("echo ") == 0) std::cout << input.substr(5) << '\n' << std::unitbuf;
    else std::cerr << input << ": command not found\n" << std::unitbuf;
    std::cout << "$ ";
  }
}
