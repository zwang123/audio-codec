#ifndef __FLAC__FLAC_SEEK_H_INCLUDED
#define __FLAC__FLAC_SEEK_H_INCLUDED

#include <cstdint>
#include <istream>
#include <vector>

namespace flac {
  //std::istream &flac_seek_frame_header(std::istream &);
  std::vector<uint8_t> flac_seek_frame_header(std::istream &);
}
#endif // __FLAC__FLAC_SEEK_H_INCLUDED
