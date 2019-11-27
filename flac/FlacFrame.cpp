#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacFrame.h"
#include "FlacUtilities.h"

namespace flac {
int FlacFrame::write(std::ostream &os) const
{
  char header_buffer[MAX_FRAME_HEADER_SIZE];
  uint8_t remainder = 0;
  unsigned remainder_digit = 0;
  auto ptr = header_buffer;
  ptr = package<16>(ptr, sync_word, remainder, remainder_digit);
  ptr = package<8>(ptr, size_rate, remainder, remainder_digit);
  ptr = package<8>(ptr, channel_bitdepth, remainder, remainder_digit);
  // TODO utf encode
  ptr = package<8>(ptr, frame_number, remainder, remainder_digit);
  // TODO block size/sample rate
  ptr = package<8>(ptr, crc8, remainder, remainder_digit);
  os.write(header_buffer, ptr - header_buffer);

  for (const auto &subframe : subframes) {
    subframe.write(os, remainder, remainder_digit);
  }

  if (remainder_digit) {
    os.put(remainder << (8 - remainder_digit));
    remainder = 0;
    remainder_digit = 0;
  }

  ptr = header_buffer;
  ptr = package<16>(ptr, crc16, remainder, remainder_digit);
  os.write(header_buffer, ptr - header_buffer);

  //TODO
  return RETURN_SUCCESS;
}
}
