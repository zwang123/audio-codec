#ifndef __FLAC__FLAC_RICE_PARTITION_H_INCLUDED
#define __FLAC__FLAC_RICE_PARTITION_H_INCLUDED

#include <cstdint>
#include <iostream>

//#include "FlacErrorCodes.h"
//#include "FlacUtilities.h"
//#include "crc.h"

namespace flac {
class FlacRicePartition {
public:
  FlacRicePartition(
      std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint_fast8_t rice_parameter_bit_size, uint16_t _partition_size) {}
  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const {return 0;}
};
}
#endif // __FLAC__FLAC_RICE_PARTITION_H_INCLUDED
