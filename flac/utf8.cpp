#include <algorithm>
#include "utf8.h"
namespace flac {
  char *utf8_encode(uint64_t x, char * const out)
  {
    char *rtn = out;
    if (x & 0xfffffff80) {
      //uint_fast8_t tmp = x & 0x3f;
      do {
        *rtn++ = 0x80 | (x & 0x3f);
        x >>= 6;
      } while (x);
      if (rtn[-1] & (~((1 << (7 - (rtn - out))) - 1) & 0x3f))
        ++rtn;
      rtn[-1] |= ~((1 << (8 - (rtn - out))) - 1);
      std::reverse(out, rtn);
    } else {
      *rtn++ = x;
      //*out++ = x;
      //rtn = out;
    }
    return rtn;
   
    //char *rtn = out;
    //if (x & 0xfffffff80) {
    //  if (x & 0xffffff800) {
    //    if (x & 0xfffff0000) {
    //      ;
    //    } else {
    //      *out++ = (x >> 12) | 0xe0;
    //      rtn = out + 2;
    //    }
    //  } else {
    //    *out++ = (x >> 6) | 0xc0;
    //    rtn = out + 1;
    //  }
    //} else {
    //  *out++ = x;
    //  rtn = out;
    //}

    //0760000000000;
    //uint64_t rtn = 0;
    //uint_fast8_t tmp = x & 0x3f;
    //x >>= 6;
    //;
  }

  uint64_t utf8_decode(uint64_t)
  {
    ;
  }
}
