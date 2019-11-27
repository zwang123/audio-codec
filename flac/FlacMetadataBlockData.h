#ifndef __FLAC__FLAC_METADATA_BLOCK_DATA_H_INCLUDED
#define __FLAC__FLAC_METADATA_BLOCK_DATA_H_INCLUDED
#include <cassert>
namespace flac {
class FlacMetadataBlockData {
public:
  virtual int read(std::istream &, uint32_t) = 0;
  virtual int write(std::ostream &) const = 0;
  virtual int write(std::ostream &os, uint32_t len) const {
    assert(len == block_length);
    return this->write(os);
  }
  virtual ~FlacMetadataBlockData() {}
protected:
  uint32_t block_length;
  FlacMetadataBlockData(uint32_t _block_length) : block_length(_block_length) {}
};
}
#endif // __FLAC__FLAC_METADATA_BLOCK_DATA_H_INCLUDED
