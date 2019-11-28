#ifndef __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacSubframeData.h"
#include "FlacUtilities.h"
#include "crc.h"

namespace flac {
//template <typename DataType>
class FlacSubframeConstant : public FlacSubframeData {
using DataType = uint32_t;
public:
  explicit FlacSubframeConstant(
      DataType _value = 0,
      uint8_t _bits_per_sample = 16)
    : value(_value)
    , bits_per_sample(_bits_per_sample) {}

  FlacSubframeConstant(
      std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint8_t _bits_per_sample) : bits_per_sample ( _bits_per_sample )
  { read(is, remainder, remainder_digit); }

  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
      override
  { read_type_n_with_remainder(is, bits_per_sample, value, 
          remainder, remainder_digit);  return RETURN_SUCCESS; }

  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const override
  {
    char buffer[sizeof(value)];
    char *ptr = buffer;
    ptr = package(ptr, bits_per_sample, value, remainder, remainder_digit);
    os.write(buffer, ptr - buffer);
    crc16_encodes(buffer, ptr, crc16);
    return RETURN_SUCCESS;
  }
private:
  DataType value;
  uint8_t bits_per_sample;
};
}
#endif // __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
