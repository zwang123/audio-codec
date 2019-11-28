#include <cstdint>
#include <istream>
#include <deque>
#include "FlacFrame.h"
#include "FlacSeek.h"
#include "crc.h"

namespace flac {
  std::deque<uint8_t> flac_seek_frame_header(std::istream &is)
  {
    uint8_t c, crc;
    std::deque<uint8_t> rtn;
    do {
      do {
        crc = 0;
        rtn.clear();
        do {
          is.get(*reinterpret_cast<char*>(&c));
        } while (is && (unsigned char) c != 0xff);
        crc8_encode(c, crc);
        rtn.push_back(c);

        is.get(*reinterpret_cast<char*>(&c));
      } while (is && c != 0xf8 && c != 0xf9);
      crc8_encode(c, crc);
      rtn.push_back(c);

      bool loop_flag = false;
      do {
        loop_flag = false;
        while (rtn.size() < FlacFrame::MAX_FRAME_HEADER_SIZE && crc) {
          is.get(*reinterpret_cast<char*>(&c));
          crc8_encode(c, crc);
          rtn.push_back(c);
        }
        if (crc) {
          rtn.pop_front();
          rtn.pop_front();
          while (rtn.size() > 1) {
            if (rtn[0] == 0xff && 
              rtn[1] == 0xf8 || rtn[1] == 0xf9) {
              crc = 0xf3;
              crc8_encode(rtn[1], crc);
              loop_flag = true;
              break;
            }
            else rtn.pop_front();
          }
        }
      } while (loop_flag);

    } while (crc);
    return rtn;
  }
}
