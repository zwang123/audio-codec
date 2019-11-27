#ifndef __FLAC__FLAC_SUBFRAME_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_H_INCLUDED

#include <cstdint>
#include "FlacSubframeConstant.h"

namespace flac {
class FlacSubframe {
public:
  FlacSubframe() : data(std::make_shared<FlacSubframeConstant<int16_t>>()) {}
  int write(std::ostream &) const;
private:
  uint8_t header = 0;
  uint32_t wasted_bits = 0;
  // data
  std::shared_ptr<class FlacSubframeData> data;
};
}
#endif // __FLAC__FLAC_SUBFRAME_H_INCLUDED
