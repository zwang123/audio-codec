#ifndef __FLAC__FLAC_SEEK_H_INCLUDED
#define __FLAC__FLAC_SEEK_H_INCLUDED

#include <cstdint>
#include <istream>
#include <deque>

namespace flac {
  std::deque<uint8_t> flac_seek_frame_header(std::istream &);
}
#endif // __FLAC__FLAC_SEEK_H_INCLUDED
