#include "FlacUtilities.h"

namespace flac {
  template <>
  std::istream &read_type_n<8>(std::istream &is, char &data, uint8_t &remainder)
  {
    if (read_char(is, data))
      remainder = 0;
    return is;
  }

  template <>
  std::istream &read_type_n<8>(std::istream &is, uint8_t &data, uint8_t &remainder)
  {
    //std::cout << __FILE__ << __LINE__ << std::endl;
    if (read_uint8(is, data))
      remainder = 0;
    return is;
  }

  template <>
  std::istream &read_type_n<16>(std::istream &is, uint16_t &data, uint8_t &remainder)
  {
    if (read_uint16(is, data))
      remainder = 0;
    return is;
  }

  template <>
  std::istream &read_type_n<24>(std::istream &is, uint32_t &data, uint8_t &remainder)
  {
    if (read_uint24(is, data))
      remainder = 0;
    return is;
  }

  template <>
  std::istream &read_type_n<32>(std::istream &is, uint32_t &data, uint8_t &remainder)
  {
    if (read_uint32(is, data))
      remainder = 0;
    return is;
  }

  template <>
  std::istream &read_type_n<64>(std::istream &is, uint64_t &data, uint8_t &remainder)
  {
    if (read_uint64(is, data))
      remainder = 0;
    return is;
  }
}
