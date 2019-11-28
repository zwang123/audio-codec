#ifndef __FLAC__UTF8_H_INCLUDED
#define __FLAC__UTF8_H_INCLUDED

#include <cstdint>
#include <istream>
#include <iterator>

namespace flac {
  char *utf8_encode(uint64_t, char * const);

  template <typename ForwardIterator>
  uint64_t utf8_decode(ForwardIterator it)
  {
    uint64_t rtn = static_cast<uint8_t>(*it++);
    uint_fast8_t n = 1;
    if (rtn & 0x80) {
      rtn ^= 0xc0;
      n = 2;
      for (uint_fast8_t curr_bit = 0x20; rtn & curr_bit; curr_bit >>= 1) {
        rtn ^= curr_bit;
        ++n;
      }

      //assert(n < 8);

      while(--n) {
        rtn <<= 6;
        rtn |= *it++ & 0x3f;
      }
    }
    return rtn;
  }

  inline
  uint64_t utf8_decode_stream(std::istream &is)
  {
    return utf8_decode(std::istream_iterator<char>(is));
  }
}

#endif // __FLAC__UTF8_H_INCLUDED
