#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacMetadataBlockStreaminfo.h"
#include "FlacUtilities.h"

namespace flac {
int FlacMetadataBlockStreaminfo::read(std::istream &is)
{
  uint16_t _min_block_size;
  uint16_t _max_block_size;
  uint32_t _min_frame_size;
  uint32_t _max_frame_size;
  uint32_t _sample_rate   ;
  uint8_t  _num_channels  ;
  uint8_t  _bits_per_sample;
  uint64_t _total_samples ;
  uint64_t _md5_1;
  uint64_t _md5_2;
  //read_type(read_type(is, _min_block_size), _max_block_size);
  //if (!is) return STREAM_ERROR;
  //read_type_n(read_type_n(is, _min_frame_size, 3), _max_frame_size, 3);
  //if (!is) return STREAM_ERROR;
  return RETURN_SUCCESS;
}
}
