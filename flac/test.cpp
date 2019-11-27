#include <iostream>
#include "FlacStream.h"

int main()
{
  flac::FlacStream flac;
  flac::FlacStream flac2("xxx.flac");
  std::cout << "Successfully exiting..." << std::endl;
  return 0;
}
