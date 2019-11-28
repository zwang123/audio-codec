#ifndef __FLAC__FLAC_SUBFRAME_VERBATIM_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_VERBATIM_H_INCLUDED

#include <iostream>
#include <valarray>
#include "FlacErrorCodes.h"
#include "FlacSubframeData.h"
#include "FlacUtilities.h"
#include "crc.h"

namespace flac {
class FlacSubframeVerbatim : public FlacSubframeData {
using DataType = uint32_t;
using value_type = std::valarray<DataType>; // ctor diff from vector
public:
  explicit FlacSubframeVerbatim(
      DataType _value = 0,
      uint8_t _bits_per_sample = 16,
      uint16_t _blocksize = 4096)
    : values(_value, _blocksize)
    , bits_per_sample(_bits_per_sample) {}

  FlacSubframeVerbatim(
      std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint8_t _bits_per_sample, uint16_t _blocksize) 
    : values(_blocksize)
    , bits_per_sample ( _bits_per_sample )
  { read(is, remainder, remainder_digit); }

  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
      override
  {
    for (auto &value : values)
      read_type_n_with_remainder(is, bits_per_sample, value, 
            remainder, remainder_digit);
    return RETURN_SUCCESS;
  }
  //{ for (value_type::size_type i = 0; i != values.size(); ++i)
  //    read_type_n_with_remainder(is, bits_per_sample, values[i], 
  //          remainder, remainder_digit); }

  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const override
  {
    char buffer[(bits_per_sample * values.size()) >> 3];
    char *ptr = buffer;
    for (const auto value : values)
      ptr = package(ptr, bits_per_sample, value, remainder, remainder_digit);
    os.write(buffer, ptr - buffer);
    crc16_encodes(buffer, ptr, crc16);
    return RETURN_SUCCESS;
  }
private:
  value_type values;
  uint8_t bits_per_sample;
};
}
#endif // __FLAC__FLAC_SUBFRAME_VERBATIM_H_INCLUDED
