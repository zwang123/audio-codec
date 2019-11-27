#ifndef __FLAC__FLAC_METADATA_BLOCK_H_INCLUDED
#define __FLAC__FLAC_METADATA_BLOCK_H_INCLUDED
#include <cstdint>
#include <utility>
#include "FlacMetadataBlockStreaminfo.h"
namespace flac {
class FlacMetadataBlock {
public:
  bool last_metadata_block_flag() const 
  {return block_flag & LAST_METADATA_BLOCK_MASK;}
  uint8_t block_type() const 
  {return block_flag & ~LAST_METADATA_BLOCK_MASK;}

  FlacMetadataBlock() : block_data(std::make_shared<FlacMetadataBlockStreaminfo>()) {}

  FlacMetadataBlock(std::istream &);

  int write(std::ostream &) const;

private:
  constexpr static uint8_t LAST_METADATA_BLOCK_MASK = 1 << 7;
  constexpr static uint8_t DEFAULT_BLOCK_FLAG = LAST_METADATA_BLOCK_MASK | 0;

  // header
  uint8_t block_flag = DEFAULT_BLOCK_FLAG;
  uint32_t block_length = FlacMetadataBlockStreaminfo::block_size;

  // data
  std::shared_ptr<class FlacMetadataBlockData> block_data;
};
}
#endif // __FLAC__FLAC_METADATA_BLOCK_H_INCLUDED
