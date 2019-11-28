#ifndef __FLAC__FLAC_SUBFRAME_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_H_INCLUDED

#include <cstdint>
#include <memory>
#include "FlacSubframeConstant.h"

namespace flac {
class FlacSubframe {
public:
  FlacSubframe() : data(std::make_unique<FlacSubframeConstant>()) {}
  FlacSubframe(std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint16_t blocksize, uint8_t bits_per_sample)
  { read(is, remainder, remainder_digit, blocksize, bits_per_sample); }
  // TODO check success

  int read(std::istream &, uint8_t &, unsigned &, uint16_t, uint8_t);
  int write(std::ostream &, uint8_t &, unsigned &, uint16_t &) const;
  uint8_t get_subframe_type() const { return header >> 1; }
private:
  uint8_t header = 0;
  uint32_t wasted_bits = 0;
  // data
  std::unique_ptr<class FlacSubframeData> data;
};
}
#endif // __FLAC__FLAC_SUBFRAME_H_INCLUDED
