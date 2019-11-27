#ifndef __FLAC__FLAC_FRAME_H_INCLUDED
#define __FLAC__FLAC_FRAME_H_INCLUDED
#include <cstdint>
#include <vector>
#include "FlacSubframe.h"
namespace flac {
class FlacFrame {
public:
  FlacFrame() : subframes(2) {}
  FlacFrame(std::istream &);

  bool variable_blocksize() const { return sync_word & 1; }

  int write(std::ostream &) const;
private:
  // header
  //bool variable_blocksize = false;
  uint16_t sync_word = 0xfff8; // fixed blocksize
  uint8_t size_rate = 0xc9; // 4096 samples, 44.1kHz
  uint8_t channel_bitdepth = 0x18; // L/R stereo, 16 bit-depth
  uint64_t frame_number = 0;
  // TODO
  // blocksize bits not allowed
  // bit sample rate not allowed
  uint8_t crc8 = 0xc2;

  std::vector<FlacSubframe> subframes;

  uint16_t crc16 = 0xb8ee;
};
}
#endif // __FLAC__FLAC_FRAME_H_INCLUDED
