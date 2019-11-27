#include <fstream>
#include <iostream>
#include "FlacStream.h"

void print_binary_file(const char *filename)
{
  std::ifstream ifs(filename, std::ios::out | std::ios::binary);
  char c;
  std::cout << std::hex;
  while (ifs.get(c)) {
    std::cout << (unsigned)(unsigned char) c << " ";
  }
  std::cout << std::dec << std::endl;
}

int main()
{
  flac::FlacStream flac;
  flac.write("yyy.flac");
  print_binary_file("yyy.flac");

  flac::FlacStream flac2("xxx.flac");
  std::cout << "Successfully exiting..." << std::endl;
  return 0;
}
