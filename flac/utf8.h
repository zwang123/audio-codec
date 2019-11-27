#ifndef __FLAC__UTF8_H_INCLUDED
#define __FLAC__UTF8_H_INCLUDED

#include <cstdint>

namespace flac {
  uint64_t utf8_encode(uint64_t);
  uint64_t utf8_decode(uint64_t);
  char *utf8_encode(uint64_t x, char * const out);
}

#endif // __FLAC__UTF8_H_INCLUDED
