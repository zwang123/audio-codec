#ifndef __FLAC__FLAC_STREAM_H_INCLUDED
#define __FLAC__FLAC_STREAM_H_INCLUDED
namespace flac {
class FlacStream {
public:
  constexpr static char[4] stream_marker = {'f', 'L', 'a', 'C'};
  FlacStream() : metadata_blocks(1), frames(1) {}
private:
  std::vector<FlacMetadataBlock> metadata_blocks;
  std::vector<FlacFrame> frames;
};
}
#endif // __FLAC__FLAC_STREAM_H_INCLUDED
