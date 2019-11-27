#include <iostream>
#include "FlacMetadataBlock.h"
#include "FlacMetadataBlockStreaminfo.h"
#include "FlacUtilities.h"

namespace flac {
FlacMetadataBlock::FlacMetadataBlock(std::istream &is)
{
  read_uint8(is, block_flag);
  read_uint24(is, block_length);
  //std::cout << __FILE__ << __LINE__ << std::endl;
  //std::cout << (int)block_flag << std::endl;
  //std::cout << (int)block_type() << std::endl;
  switch (block_type()) {
    case 0: 
      block_data = std::make_shared<FlacMetadataBlockStreaminfo>
        (is, block_length);
      break;
    default:
      // TODO
      break;
  }
}
}
