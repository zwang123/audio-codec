#ifndef __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED
#define __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED

#include <array>
#include <cassert>
#include <cstdint>
#include "FlacMetadataBlockData.h"

namespace flac {
class FlacMetadataBlockStreaminfo : public FlacMetadataBlockData {
public:
  using md5_type = std::array<uint64_t, 2>;

  constexpr static uint32_t block_size = 34;

  explicit FlacMetadataBlockStreaminfo(
    uint16_t _min_block_size  = 0x1000,
    uint16_t _max_block_size  = 0x1000,
    uint32_t _min_frame_size  = 0,
    uint32_t _max_frame_size  = 0,
    uint32_t _sample_rate     = 44100,
    uint8_t  _num_channels    = 2,
    uint8_t  _bits_per_sample = 16,
    uint64_t _total_samples   = 0,
    md5_type _md5             = md5_type{{}}
  ) : 
    min_block_size (_min_block_size ),
    max_block_size (_max_block_size ), 
    min_frame_size (_min_frame_size ), 
    max_frame_size (_max_frame_size ), 
    sample_rate    (_sample_rate    ),
    num_channels   (_num_channels   ),
    bits_per_sample(_bits_per_sample),
    total_samples  (_total_samples  ),
    md5(_md5) {}

  FlacMetadataBlockStreaminfo(std::istream &is, uint32_t _block_size)
  {
    assert(_block_size == this->block_size);
    read(is);
    assert(is);
  }

  int read(std::istream &);

private:
  uint16_t min_block_size;
  uint16_t max_block_size;
  uint32_t min_frame_size;
  uint32_t max_frame_size;
  uint32_t sample_rate;
  uint8_t num_channels;
  uint8_t bits_per_sample;
  uint64_t total_samples;
  md5_type md5;
};
}
#endif // __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED
