#include "../crc.h"
#include <cassert>
#include <cstdint>
#include <iostream>

int main()
{
  uint8_t codes[] ={0xff, 0xf8, 0xc9, 0x18, 0}; 
  uint8_t codes2[12] ={0xff, 0xf8, 0xc9, 0x18, 0, 0xc2};
  uint16_t codes3[] ={0xfff8, 0xc918, 0xc2};
  uint8_t crc8 = 0;
  uint16_t crc16 = 0;
  uint8_t crc8_2 = 0;

  std::cout << std::hex;
  for (const auto c : codes) {
    std::cout << (unsigned)flac::crc8_encode(c, crc8) << " ";
  }
  std::cout << std::endl;
  for (const auto c : codes2) {
    flac::crc16_encode(c, crc16);
  }
  for (const auto c : codes3) {
    flac::crc_encode(c, crc8_2, (uint8_t)0x7);
  }

  std::cout << "crc8: " << (unsigned)crc8 << std::endl;
  std::cout << "crc16: " << (unsigned)crc16 << std::endl;
  std::cout << "crc8 2: " << (unsigned)crc8_2 << std::endl;

  assert(crc8 == 0xc2);
  assert(crc16 == 0xb8ee);
  assert(crc8_2 == 0);

  return 0;
}

