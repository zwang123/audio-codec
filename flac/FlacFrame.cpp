#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacFrame.h"
#include "FlacUtilities.h"

namespace flac {
  const uint32_t FlacFrame::sample_rate_array[12] = { 0, 88200, 176400, 192000,
    8000, 16000, 22050, 24000, 32000, 44100, 48000, 96000};

uint16_t FlacFrame::set_subframe_blocksize(std::istream &is) { 
  if (size_rate & 0x80) {
    blocksize_bitsize = 0;
    blocksize = (0x100 << ((size_rate & 0x70) >> 4));
  } else if (size_rate & 0x40 && size_rate & 0x20) {
    blocksize_bitsize = 8 << ((size_rate >> 4) & 1);
    uint8_t remainder = 0;
    read_type_n(is, blocksize_bitsize, blocksize, remainder);
  } else if (size_rate & 0x40 || size_rate & 0x20) {
    blocksize_bitsize = 0;
    blocksize = 144 << (size_rate >> 4);
  } else {
    blocksize_bitsize = 0;
    assert(size_rate >> 4);
    blocksize = 192;
  }
  return blocksize;
}

uint32_t FlacFrame::set_sample_rate(std::istream &is)
{
  auto rate_idx = (size_rate & 0xf);
  if (rate_idx < 12) {
    samplerate_bitsize = 0;
    samplerate = sample_rate_array[rate_idx];
    // TODO rate_idx == 0
  } else {
    assert(rate_idx != 0xf);
    samplerate_bitsize = 8 << static_cast<bool>(rate_idx & 3);
    uint8_t remainder = 0;
    read_type_n(is, samplerate_bitsize, samplerate, remainder);
    if (rate_idx & 2) samplerate *= 10;
    else if (!(rate_idx & 1)) samplerate *= 1000;
  }
  
  return samplerate;
}

int FlacFrame::read(std::istream &is)
{
  read_uint16(is, sync_word);
  read_uint8(is, size_rate);
  read_uint8(is, channel_bitdepth);
  // TODO
  uint8_t remainder = 0;
  read_type_n<8>(is, frame_number, remainder);
  set_subframe_blocksize(is);
  set_sample_rate(is);
  read_uint8(is, crc8);
  // TODO subframe
  // TODO footer
}

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
