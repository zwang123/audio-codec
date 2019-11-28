#ifndef __FLAC__FLAC_SUBFRAME_FIXED_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_FIXED_H_INCLUDED

#include <iostream>
#include <valarray>
#include "FlacResidual.h"
#include "FlacSubframeData.h"

#include "FlacErrorCodes.h"
#include "FlacUtilities.h"
#include "crc.h"

namespace flac {
class FlacSubframeFixed : public FlacSubframeData {
using DataType = uint32_t;
using value_type = std::valarray<DataType>; // ctor diff from vector
public:
  FlacSubframeFixed(
      std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint8_t _bits_per_sample, uint16_t _blocksize, uint_fast8_t _order)
    : bits_per_sample(_bits_per_sample)
    , warm_up_samples(_order)
    , residual(_blocksize, _order) 
  { read(is, remainder, remainder_digit); }
  //TODO

  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
      override
  { 
    for (auto &value : warm_up_samples)
      read_type_n_with_remainder(is, bits_per_sample, value, 
            remainder, remainder_digit); 
    residual.read(is, remainder, remainder_digit);
    return RETURN_SUCCESS;
  }
  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const override
  {
    char buffer[(bits_per_sample * warm_up_samples.size()) >> 3];
    char *ptr = buffer;
    for (const auto value : warm_up_samples)
      ptr = package(ptr, bits_per_sample, value, remainder, remainder_digit);
    os.write(buffer, ptr - buffer);
    crc16_encodes(buffer, ptr, crc16);
    residual.write(os, remainder, remainder_digit, crc16);
    return RETURN_SUCCESS;
  }
private:
  uint8_t bits_per_sample;
  value_type warm_up_samples;
  FlacResidual residual;
};
}
#endif // __FLAC__FLAC_SUBFRAME_FIXED_H_INCLUDED
