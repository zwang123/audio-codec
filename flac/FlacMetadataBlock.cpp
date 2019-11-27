#include <iostream>
#include "FlacErrorCodes.h"
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

  char buffer[1024];
  uint32_t remaining_len = block_length;

  switch (block_type()) {
    case 0: 
      block_data = std::make_shared<FlacMetadataBlockStreaminfo>
        (is, block_length);
      break;
    default:
      // TODO
      while (remaining_len > 1024) {
        is.read(buffer, 1024);
        remaining_len -= 1024;
      }
      is.read(buffer, remaining_len);
      break;
  }
}

int FlacMetadataBlock::write(std::ostream &os) const
{
  // TODO
  return RETURN_SUCCESS;
}

}
