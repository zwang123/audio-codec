#ifndef __FLAC__FLAC_FRAME_H_INCLUDED
#define __FLAC__FLAC_FRAME_H_INCLUDED
#include <cstdint>
#include <vector>
#include "FlacSubframe.h"

//#include <iostream>

namespace flac {
class FlacFrame {
public:
  FlacFrame() : subframes(2) {
    //for (size_rate = 0x10; size_rate; size_rate += 0x10) {
    //  std::cout << (unsigned)size_rate << " " << 
    //    set_subframe_blocksize() << std::endl;
    //}
    //size_rate = 0xc9;
  }
  FlacFrame(std::istream &is) {read(is);} //TODO check validity

  bool variable_blocksize() const { return sync_word & 1; }

  int read(std::istream &);
  int write(std::ostream &) const;
private:
  static const uint32_t sample_rate_array[12];

  uint16_t set_subframe_blocksize(std::istream &);
  uint32_t set_sample_rate(std::istream &);

  constexpr static uint32_t MAX_FRAME_HEADER_SIZE = 16;
  // header
  uint16_t sync_word = 0xfff8; // fixed blocksize
  uint8_t size_rate = 0xc9; // 4096 samples, 44.1kHz
  uint8_t channel_bitdepth = 0x18; // L/R stereo, 16 bit-depth
  uint64_t frame_number = 0;

  uint16_t blocksize = 4096;
  unsigned blocksize_bitsize = 0;

  uint32_t samplerate = 44100;
  unsigned samplerate_bitsize = 0;

  // TODO
  // bit sample rate not allowed

  uint8_t crc8 = 0xc2;

  std::vector<FlacSubframe> subframes;

  // footer
  uint16_t crc16 = 0xb8ee;
};
}
#endif // __FLAC__FLAC_FRAME_H_INCLUDED
