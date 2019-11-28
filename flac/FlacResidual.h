#ifndef __FLAC__FLAC_RESIDUAL_H_INCLUDED
#define __FLAC__FLAC_RESIDUAL_H_INCLUDED

#include <cstdint>
#include <iostream>

namespace flac {
class FlacResidual {
public:
  FlacResidual(uint16_t _blocksize) {}
  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
  {return 0;}
  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const {return 0;}
};
}
#endif // __FLAC__FLAC_RESIDUAL_H_INCLUDED
