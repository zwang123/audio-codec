#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacSubframe.h"
#include "FlacUtilities.h"

namespace flac {
int FlacSubframe::write(std::ostream &os, 
    uint8_t &remainder, unsigned &remainder_digit) const
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

  os.write(buffer, ptr - buffer);

  data->write(os, remainder, remainder_digit);

  return RETURN_SUCCESS;
}
}
