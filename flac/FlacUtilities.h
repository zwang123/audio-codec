#ifndef __FLAC__FLAC_UTILITIES_H_INCLUDED
#define __FLAC__FLAC_UTILITIES_H_INCLUDED

#include <cstddef>
#include <iostream>

namespace flac {
  //// Assume little endian
  //template <typename T>
  //std::istream &read_type(std::istream &is, T &data) {
  //  is.read(reinterpret_cast<char*>(&data), sizeof(data));
  //  return is;
  //}
  //template <typename T>
  //std::istream &read_type_n(std::istream &is, T &data, unsigned size) {
  //  is.read(reinterpret_cast<char*>(&data) + sizeof(data) - size, size);
  //  return is;
  //}

  //bugged, sign convertion
  //must use unsigned char
  // TODO
  //// Flac is big endian
  //template <std::size_t size, typename T>
  //std::istream &read_type_n(std::istream &is, T &data, unsigned size, 
  //    uint8_t &remainder);

  //inline
  //std::istream &read_uint8(std::istream &is, uint8_t &data)
  //{
  //  char buffer;
  //  is.read(reinterpret_cast<char*>(&data), 1);
  //  return is;
  //}

  //inline
  //std::istream &read_uint16(std::istream &is, uint16_t &data)
  //{
  //  char buffer[2];
  //  is.read(buffer, 2);
  //  if (is) {
  //    data = buffer[0];
  //    data <<= 8;
  //    data |= buffer[1];
  //  }
  //  return is;
  //}

  //inline
  //std::istream &read_uint24(std::istream &is, uint32_t &data)
  //{
  //  char buffer[3];
  //  is.read(buffer, 3);
  //  if (is) {
  //    data = buffer[0];
  //    data <<= 8;
  //    data |= buffer[1];
  //    data <<= 8;
  //    data |= buffer[2];
  //  }
  //  return is;
  //}

  //inline
  //std::istream &read_uint32(std::istream &is, uint32_t &data)
  //{
  //  char buffer[4];
  //  is.read(buffer, 4);
  //  if (is) {
  //    data = buffer[0];
  //    data <<= 8;
  //    data |= buffer[1];
  //    data <<= 8;
  //    data |= buffer[2];
  //    data <<= 8;
  //    data |= buffer[3];
  //  }
  //  return is;
  //}

  //template <typename T>
  //std::ostream &write_type(std::ostream &os, T &data) {
  //  os.write(reinterpret_cast<char*>(&data), sizeof(data));
  //  return os;
  //}
}
#endif // __FLAC__FLAC_UTILITIES_H_INCLUDED
