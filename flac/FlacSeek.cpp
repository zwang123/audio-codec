#include <cstdint>
#include <istream>
#include <vector>
#include "FlacFrame.h"
#include "FlacSeek.h"
#include "crc.h"

namespace flac {
  std::vector<uint8_t> flac_seek_frame_header(std::istream &is)
  {
    uint8_t c, crc;
    std::vector<uint8_t> rtn;
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

      while (rtn.size() < FlacFrame::MAX_FRAME_HEADER_SIZE && crc) {
        is.get(*reinterpret_cast<char*>(&c));
        crc8_encode(c, crc);
        rtn.push_back(c);
      }
    } while (crc);
    return rtn;
  }
  //std::istream &flac_seek_frame_header(std::istream &is)
  //{
  //  uint8_t c0, c1, crc;
  //  unsigned header_len;
  //  do {
  //    do {
  //      do {
  //        crc = 0;
  //        is.get(*reinterpret_cast<char*>(&c0));
  //        crc8_encode(c0, crc);
  //      } while (is && (unsigned char) c0 != 0xff);
  //      is.get(*reinterpret_cast<char*>(&c1));
  //      crc8_encode(c1, crc);
  //    } while (is && c1 != 0xf8 && c1 != 0xf9);
  //    header_len = 2;
  //    while (header_len < FlacFrame::MAX_FRAME_HEADER_SIZE && crc) {
  //      // what if here we have fff8/fff9??
  //      is.get(*reinterpret_cast<char*>(&c0));
  //      crc8_encode(c0, crc);
  //      ++header_len;
  //    }
  //  } while (crc);
  //  return is;
  //}
}
