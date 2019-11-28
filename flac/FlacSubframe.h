#ifndef __FLAC__FLAC_SUBFRAME_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_H_INCLUDED

#include <cstdint>
#include <memory>
#include "FlacSubframeConstant.h"

namespace flac {
class FlacSubframe {
public:
  FlacSubframe() : data(std::make_unique<FlacSubframeConstant<int16_t>>()) {}
  int write(std::ostream &, uint8_t &, unsigned &, uint16_t &) const;
private:
  uint8_t header = 0;
  uint32_t wasted_bits = 0;
  // data
  std::unique_ptr<class FlacSubframeData> data;
};
}
#endif // __FLAC__FLAC_SUBFRAME_H_INCLUDED
