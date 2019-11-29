#ifndef __FLAC__UNARY_CODEC_H_INCLUDED
#define __FLAC__UNARY_CODEC_H_INCLUDED

#include <cstdint>
#include <istream>
#include "FlacUtilities.h"

namespace flac {
  // 0 -> 1
  // 1 -> 01
  // 2 -> 001
  inline
  char *unary_encode(char *ptr, uint32_t num, 
      uint8_t &remainder, unsigned &remainder_digit) 
  {
    ++num;
    while (num > 64) {
      ptr = package<64>(ptr, 0ull, remainder, remainder_digit);
      num -= 64;
    }
    ptr = package(ptr, num, 1ull, remainder, remainder_digit);
    return ptr;
  }

  //   1 -> 0
  //  01 -> 1
  // 001 -> 2
  template <typename T>
  std::istream &unary_decode(std::istream &is, T &num, 
      uint8_t &remainder, unsigned &remainder_digit) 
  {
    num = -1;
    uint8_t bit = 0;
    do {
    //std::cout << __FILE__ << __LINE__ << std::endl;
      read_type_n_with_remainder(is, 1, bit, remainder, remainder_digit);
    //std::cout << __FILE__ << __LINE__ << std::endl;
      ++num;
    } while (bit == 0);
    //std::cout << "num" << num << std::endl;
    return is;
  }
}
#endif // __FLAC__UNARY_CODEC_H_INCLUDED
