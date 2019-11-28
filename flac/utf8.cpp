#include <algorithm>
#include "utf8.h"
namespace flac {
  char *utf8_encode(uint64_t x, char * const out)
  {
    char *rtn = out;
    if (x & 0xfffffff80) {
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
    }
    return rtn;
  }
}
