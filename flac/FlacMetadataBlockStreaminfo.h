#ifndef __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED
#define __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED

#include <array>
#include <cstdint>
#include "FlacMetadataBlockData.h"

namespace flac {
class FlacMetadataBlockStreaminfo : public FlacMetadataBlockData {
public:
  constexpr static uint32_t block_size = 34;
  FlacMetadataBlockStreaminfo() : md5{{}} {}
private:
  uint16_t min_block_size = 16;
  uint16_t max_block_size = 65535;
  uint32_t min_frame_size = 0;
  uint32_t max_frame_size = 0;
  uint32_t sample_rate = 44100;
  uint8_t num_channels = 2;
  uint8_t bits_per_sample = 16;
  uint64_t total_samples = 0;
  std::array<uint32_t, 4> md5;
};
}
#endif // __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED
