#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include "FlacStream.h"


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
    metadata_blocks.emplace_back(file);

    //if (!file) {
    //  throw std::runtime_error(std::string("Cannot open file ") + filename);
    //}
    file.close();
  }
}
