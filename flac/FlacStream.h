#ifndef __FLAC__FLAC_STREAM_H_INCLUDED
#define __FLAC__FLAC_STREAM_H_INCLUDED

#include <vector>
#include "FlacFrame.h"
#include "FlacMetadataBlock.h"

namespace flac {
class FlacStream {
public:
  constexpr static char stream_marker[4] = {'f', 'L', 'a', 'C'};
  FlacStream() : metadata_blocks(1), frames(1) {}
  explicit FlacStream(const char *);
  int write(const char *) const;
private:
  std::vector<FlacMetadataBlock> metadata_blocks;
  std::vector<FlacFrame> frames;
};
}
#endif // __FLAC__FLAC_STREAM_H_INCLUDED
