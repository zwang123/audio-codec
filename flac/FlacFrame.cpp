#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacFrame.h"
#include "FlacMetadataBlockStreaminfo.h"
#include "FlacUtilities.h"
#include "crc.h"
#include "utf8.h"

//streaminfo:
//sample rate
//bit per sample

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
    ++blocksize; // TODO might overflow
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
    if (rate_idx == 0) {
      samplerate = pStreamInfo->sample_rate;
    }
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

uint8_t FlacFrame::set_bits_per_sample()
{
  uint8_t bit_size_code = (channel_bitdepth >> 1) & 7;
  bits_per_sample = bit_size_code ?
    ((bit_size_code < 4) + bit_size_code) << 2 :
    pStreamInfo -> bits_per_sample;
  return bits_per_sample;
}

void FlacFrame::test_pStreamInfo()
{
  size_rate &= 0xf0;
  channel_bitdepth &= 0xf0;
  set_sample_rate(std::cin);
  set_bits_per_sample();
  std::cout << __FILE__ << __LINE__ << std::endl;
  std::cout << (unsigned) samplerate << std::endl;
  std::cout << (unsigned) bits_per_sample << std::endl;
}

int FlacFrame::read(std::istream &is)
{
  read_uint16(is, sync_word);
  std::cout << "sync: " << (unsigned)sync_word << std::endl;
  read_uint8(is, size_rate);
  read_uint8(is, channel_bitdepth);
  std::cout << "channel_bitdepth: " << (unsigned)channel_bitdepth << std::endl;
  set_bits_per_sample();

  //{char c;is.get(c);std::cout<<(unsigned)(uint8_t)c<<std::endl;is.unget();}
  frame_number = utf8_decode_stream(is);
  std::cout << "frame_number: " << frame_number << std::endl;
  //{is.unget();char c;is.get(c);std::cout<<(unsigned)(uint8_t)c<<std::endl;}

  set_subframe_blocksize(is);
  set_sample_rate(is);
  read_uint8(is, crc8);

  std::cout << "crc8: " << (unsigned)crc8 << std::endl;
  //DEBUG
  //test_pStreamInfo();

  uint8_t remainder = 0;
  unsigned remainder_digit = 0;
  subframes.clear();
  // TODO check num_channels consistency?
  for (auto num_channels = pStreamInfo->num_channels;
       num_channels--;) {
    subframes.emplace_back(is, remainder, remainder_digit,
        blocksize, bits_per_sample);
  }

  assert(remainder == 0); // pad 0 to align

  read_uint16(is, crc16);

  std::cout << "crc16: " << crc16 << std::endl<<std::endl;

  return RETURN_SUCCESS;
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

  // assume remainder_digit == 0
  ptr = utf8_encode(frame_number, ptr);
  
  ptr = package(ptr, blocksize_bitsize, blocksize - 1, 
      remainder, remainder_digit);
  
  auto samplerate_record = samplerate;
  if (size_rate & 2) samplerate_record /= 10;
  else if (!(size_rate & 1)) samplerate_record /= 1000;
  ptr = package(ptr, samplerate_bitsize, samplerate_record,
      remainder, remainder_digit);
  
  ptr = package<8>(ptr, crc8, remainder, remainder_digit);

  uint8_t _crc8 = 0;
  uint16_t _crc16 = 0;
  for (const char *p = header_buffer; p != ptr; ++p) {
    crc8_encode(*p, _crc8);
    crc16_encode(*p, _crc16);
  }

  assert(_crc8 == 0);

  os.write(header_buffer, ptr - header_buffer);

  for (const auto &subframe : subframes) {
    subframe.write(os, remainder, remainder_digit, _crc16);
  }

  if (remainder_digit) {
    os.put(remainder <<= (8 - remainder_digit));
    crc16_encode(remainder, _crc16);
    remainder = 0;
    remainder_digit = 0;
  }

  assert(_crc16 == crc16);

  ptr = header_buffer;
  ptr = package<16>(ptr, crc16, remainder, remainder_digit);
  os.write(header_buffer, ptr - header_buffer);

  //TODO
  return RETURN_SUCCESS;
}
}
