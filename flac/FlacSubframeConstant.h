#ifndef __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacSubframeData.h"
#include "FlacUtilities.h"
#include "crc.h"

namespace flac {
template <typename DataType>
class FlacSubframeConstant : public FlacSubframeData {
public:
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
  DataType value = 0;
  uint8_t bits_per_sample = 16;
};
}
#endif // __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
