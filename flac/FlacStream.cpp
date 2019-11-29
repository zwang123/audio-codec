#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include "FlacFrame.h"
#include "FlacMetadataBlock.h"
#include "FlacStream.h"

#include <iostream>
#include "FlacSeek.h"

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

    //std::cout << metadata_blocks.size() << std::endl;

    std::cout << std::hex;

    // TODO
    frames.emplace_back(file, std::dynamic_pointer_cast<
        const FlacMetadataBlockStreaminfo>
        (metadata_blocks.front().get_block_data()));


    flac_seek_frame_header(file);
    flac_seek_frame_header(file);
    flac_seek_frame_header(file);

    //for (int k = 0; k < 10; ++k) {
    //  std::cout << "FRAME HEADER START" << std::endl;
    //  for (const auto c : flac_seek_frame_header(file))
    //    std::cout << (unsigned)c << std::endl;
    //  std::cout << "FRAME HEADER END" << std::endl;
    //  for (int i = 0; i < 10; ++i) {
    //    file.read(buffer, 4);
    //    std::cout << (unsigned)(unsigned char)buffer[0] << std::endl;
    //    std::cout << (unsigned)(unsigned char)buffer[1] << std::endl;
    //    std::cout << (unsigned)(unsigned char)buffer[2] << std::endl;
    //    std::cout << (unsigned)(unsigned char)buffer[3] << std::endl;
    //  }
    //}


    //if (!file) {
    //  throw std::runtime_error(std::string("Cannot open file ") + filename);
    //}
    file.close();
  }

  int FlacStream::write(const char *filename) const
  {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file) {
      throw std::runtime_error(std::string("Cannot open file ") + filename);
    }

    //file.write(stream_marker, 4);
    file.write("fLaC", 4);
    for (const auto &metablock : metadata_blocks)
      metablock.write(file);
    for (const auto &frame : frames)
      frame.write(file);

    if (!file) {
      throw std::runtime_error(std::string("Cannot write file ") + filename);
    }

    file.close();
  }
}
