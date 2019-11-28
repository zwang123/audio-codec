#ifndef __FLAC__CRC_H_INCLUDED
#define __FLAC__CRC_H_INCLUDED
#include <cstddef>
#include <cstdint>

//#include <iostream>

namespace flac {
  template <typename InputType, typename DivisorType,
            std::size_t inputsize = sizeof(InputType) << 3,
            std::size_t outputsize = sizeof(DivisorType) << 3
           >
  DivisorType crc_encode(InputType input, 
                         DivisorType &output, 
                         const DivisorType divisor)
  {
    // only lower bits are valid
    static_assert(inputsize <= sizeof(input) << 3, "wrong inputsize");
    static_assert(outputsize <= sizeof(output) << 3, "wrong outputsize");

    if (inputsize < outputsize) {
      InputType mask = output >> (outputsize - inputsize);
      input ^= mask;
      output <<= inputsize;
    } else {
      // more common
      InputType mask = static_cast<InputType>(output)
        << (inputsize - outputsize);
      input ^= mask;
      output = 0;
    }

    //std::cout << std::hex;
    //std::cout << __FILE__ << __LINE__ << std::endl;
    //std::cout << "in: " << (unsigned)input << std::endl;
    //std::cout << "out: " << (unsigned)output << std::endl;

    unsigned top_bit_pos = inputsize - 1;
    InputType top_bit_mask = 1 << top_bit_pos;
    while (input) {
      if (input & top_bit_mask) {
        input ^= top_bit_mask;
        if (top_bit_pos < outputsize) {
          // more common
          input ^= static_cast<InputType>(divisor
            >> (outputsize - top_bit_pos));
          output ^= divisor << top_bit_pos;
        } else {
          input ^= static_cast<InputType>(divisor)
            << (top_bit_pos - outputsize);
        }
      }
      --top_bit_pos;
      top_bit_mask >>= 1;
    }

    return output;
  }

  // CRC-8-CCITT
  // divisor = 0x107
  inline
  uint8_t crc8_encode(uint8_t input, uint8_t &output, 
      const uint8_t divisor = 7)
  {
    return crc_encode(input, output, divisor);
  }
  // CRC-16-IBM
  // divisor = 0x18005
  inline
  uint16_t crc16_encode(uint8_t input, uint16_t &output, 
      const uint16_t divisor = 0x8005)
  {
    return crc_encode(input, output, divisor);
  }

  inline
  template <typename ForwardIterator>
  void crc16_encodes(ForwardIterator beg, ForwardIterator end,
      uint16_t &output, const uint16_t divisor = 0x8005)
  {
    while (beg != end)
      crc16_encode(*beg++, output, divisor);
  }
}
#endif // __FLAC__CRC_H_INCLUDED
