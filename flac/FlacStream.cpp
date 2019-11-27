#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include "FlacStream.h"

#include <iostream>

namespace flac{
  FlacStream::FlacStream(const char *filename)
  {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
      throw std::runtime_error(std::string("Cannot open file ") + filename);
    }

    char buffer[4];
    file.read(buffer, 4);
    if (strncmp(buffer, "fLaC", 4) != 0)
      throw std::runtime_error(std::string("File ") + filename + 
          " is not a flac file");
    do {
      metadata_blocks.emplace_back(file);
    } while (!metadata_blocks.back().last_metadata_block_flag());

    std::cout << metadata_blocks.size() << std::endl;
    file.read(buffer, 4);
    std::cout << (int)buffer[0] << std::endl;
    std::cout << (int)buffer[1] << std::endl;

    //if (!file) {
    //  throw std::runtime_error(std::string("Cannot open file ") + filename);
    //}
    file.close();
  }
}
