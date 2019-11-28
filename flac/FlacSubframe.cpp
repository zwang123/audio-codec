#include <iostream>
#include <stdexcept>
#include "FlacErrorCodes.h"
#include "FlacSubframe.h"
#include "FlacSubframeConstant.h"
#include "FlacSubframeFixed.h"
#include "FlacSubframeLpc.h"
#include "FlacSubframeVerbatim.h"
#include "FlacUtilities.h"
#include "crc.h"

namespace flac {
int FlacSubframe::read
  (std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint16_t blocksize, uint8_t bits_per_sample)
{
  read_type_n_with_remainder(is, 8, header, remainder, remainder_digit);
  wasted_bits = 0;
  if (header & 1) {
    uint8_t bit = 0;
    do {
      read_type_n_with_remainder(is, 1, bit, remainder, remainder_digit);
      ++wasted_bits;
    } while (bit == 0);
  }

  uint8_t subframe_type = header >> 1;
  std::cout << (unsigned) subframe_type << std::endl;

  if (subframe_type & 0x20) {
    // LPC
    uint_fast8_t order = (subframe_type & 0x1f) + 1;
    data = std::make_unique<FlacSubframeLpc>(is, remainder,
        remainder_digit, bits_per_sample, blocksize, order);
  } else if (subframe_type & 8) {
    // FIXED
    uint_fast8_t order = subframe_type & 7;
    data = std::make_unique<FlacSubframeFixed>(is, remainder,
        remainder_digit, bits_per_sample, blocksize, order);
  } else if (subframe_type == 0) {
    // CONST
    data = std::make_unique<FlacSubframeConstant>(is, remainder,
        remainder_digit, bits_per_sample);
  } else if (subframe_type == 1) {
    // VERB
    data = std::make_unique<FlacSubframeVerbatim>(is, remainder,
        remainder_digit, bits_per_sample, blocksize);
  } else {
    // ERROR
    throw std::runtime_error("unknown subframe type");
  }
  
  // TODO error check
  return RETURN_SUCCESS;
}

int FlacSubframe::write(std::ostream &os, 
    uint8_t &remainder, unsigned &remainder_digit, uint16_t &crc16) const
{
  char buffer[(wasted_bits + 15) >> 3];
  char *ptr = buffer;
  ptr = package<8>(ptr, header, remainder, remainder_digit);

  uint32_t remaining_wasted_bits = wasted_bits;
  while (remaining_wasted_bits > 64) {
    ptr = package<64>(ptr, 0ull, remainder, remainder_digit);
    remaining_wasted_bits -= 64;
  }
  ptr = package(ptr, remaining_wasted_bits, 1ull, remainder, remainder_digit);

  crc16_encodes(buffer, ptr, crc16);
  os.write(buffer, ptr - buffer);

  data->write(os, remainder, remainder_digit, crc16);

  return RETURN_SUCCESS;
}
}
