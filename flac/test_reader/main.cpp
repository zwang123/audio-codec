#include <fstream>
#include <iostream>

int main()
{
  std::ifstream ifs ("xxx.flac");
  char buffer[42];
  ifs.read(buffer, 42);
  std::cout << std::hex;
  for (auto x : buffer) {
    std::cout << (unsigned)(unsigned char)x << " ";
  }
  std::cout << std::endl;
  std::basic_ifstream<unsigned char> uifs;
  return 0;
}
