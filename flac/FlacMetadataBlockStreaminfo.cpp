#include <cassert>
#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacMetadataBlockStreaminfo.h"
#include "FlacUtilities.h"

namespace flac {
int FlacMetadataBlockStreaminfo::read(std::istream &is, uint32_t _block_size)
{
  assert(_block_size == this->block_size);
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
  read_uint16(read_uint16(is, _min_block_size), _max_block_size);
  if (!is) return STREAM_ERROR;
  read_uint24(read_uint24(is, _min_frame_size), _max_frame_size);

  uint8_t remainder = 0;
  read_type_n<20>(is, _sample_rate, remainder);
  _num_channels = ((remainder & 7) >> 1) + 1;
  remainder &= 1;
  read_type_n_with_remainder<5, 1>(is, _bits_per_sample, remainder);
  ++_bits_per_sample;
  read_type_n_with_remainder<36, 4>(is, _total_samples, remainder);
  read_uint64(read_uint64(is, _md5_1), _md5_2);

  //std::cout << __FILE__ << __LINE__ << std::endl;

  if (!is) return STREAM_ERROR;

  min_block_size  = _min_block_size;
  max_block_size  = _max_block_size;
  min_frame_size  = _min_frame_size;
  max_frame_size  = _max_frame_size;
  sample_rate     = _sample_rate   ;
  num_channels    = _num_channels  ;
  bits_per_sample = _bits_per_sample;
  total_samples   = _total_samples ;
  md5[0]          = _md5_1;
  md5[1]          = _md5_2;

  std::cout << min_block_size  << " "
            << max_block_size  << " " 
            << min_frame_size  << " " 
            << max_frame_size  << " " 
            << sample_rate     << " " 
            << static_cast<unsigned>(num_channels   ) << " " 
            << static_cast<unsigned>(bits_per_sample) << " " 
            << total_samples   << " " 
            << md5[0]          << " " 
            << md5[1]          << " " 
            << std::endl;



  return RETURN_SUCCESS;
}

int FlacMetadataBlockStreaminfo::write(std::ostream &os) const
{
  char buffer[block_size];
  uint8_t remainder = 0;
  unsigned remainder_digit = 0;
  char *ptr = buffer;
  ptr = package<16>(ptr, min_block_size, remainder, remainder_digit);
  ptr = package<16>(ptr, max_block_size, remainder, remainder_digit);
  ptr = package<24>(ptr, min_frame_size, remainder, remainder_digit);
  ptr = package<24>(ptr, max_frame_size, remainder, remainder_digit);
  ptr = package<20>(ptr, sample_rate, remainder, remainder_digit);
  ptr = package<3>(ptr, num_channels - 1, remainder, remainder_digit);
  ptr = package<5>(ptr, bits_per_sample - 1, remainder, remainder_digit);
  ptr = package<36>(ptr, total_samples, remainder, remainder_digit);
  ptr = package<64>(ptr, md5[0], remainder, remainder_digit);
  ptr = package<64>(ptr, md5[1], remainder, remainder_digit);
  os.write(buffer, block_size);
  return RETURN_SUCCESS;
}
}