#ifndef __FLAC__FLAC_SUBFRAME_LPC_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_LPC_H_INCLUDED

#include <iostream>
#include <valarray>
#include "FlacSubframeData.h"

namespace flac {
class FlacSubframeLpc : public FlacSubframeData {
public:
  FlacSubframeLpc(
      std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint8_t _bits_per_sample, uint16_t _blocksize, uint_fast8_t _order) {}
  //TODO

  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
      override {return 0;}
  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const override {return 0;}
};
}
#endif // __FLAC__FLAC_SUBFRAME_LPC_H_INCLUDED
