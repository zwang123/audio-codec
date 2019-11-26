#include <fstream>
#include <stdexcept>
#include <string>
#include "FlacStream.h"


namespace flac{
  FlacStream::FlacStream(const char *filename)
  {
    std::ifstream file(filename);
    if (!file) {
      throw std::runtime_error(std::string("Cannot open file ") + filename);
    }
    file.close();
  }
}
