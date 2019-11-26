#ifndef __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED
#define __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED
#include "FlacMetadataBlockData.h"
namespace flac {
class FlacMetadataBlockStreaminfo : public FlacMetadataBlockData {
public:
  constexpr static uint32_t block_size = 34;
};
}
#endif // __FLAC__FLAC_METADATA_BLOCK_STREAMINFO_H_INCLUDED
