#ifndef __FLAC__FLAC_UTILITIES_H_INCLUDED
#define __FLAC__FLAC_UTILITIES_H_INCLUDED

#include <cstddef>
#include <iostream>

namespace flac {
  // Flac is big endian
  
  inline
  std::istream &read_char(std::istream &is, char &data)
  {
    char buffer;
    is.get(buffer);
    if (is)
      data = buffer;
    return is;
  }

  inline
  std::istream &read_uint8(std::istream &is, uint8_t &data)
  {
    char buffer;
    is.get(buffer);
    if (is)
      data = buffer;
    return is;
  }

  inline
  std::istream &read_uint16(std::istream &is, uint16_t &data)
  {
    unsigned char buffer[2];
    is.read(reinterpret_cast<char*>(buffer), 2);
    if (is) {
      data = buffer[0];
      data <<= 8;
      data |= buffer[1];
    }
    return is;
  }

  inline
  std::istream &read_uint24(std::istream &is, uint32_t &data)
  {
    unsigned char buffer[3];
    is.read(reinterpret_cast<char*>(buffer), 3);
    if (is) {
      data = buffer[0];
      data <<= 8;
      data |= buffer[1];
      data <<= 8;
      data |= buffer[2];
    }
    return is;
  }

  inline
  std::istream &read_uint32(std::istream &is, uint32_t &data)
  {
    unsigned char buffer[4];
    is.read(reinterpret_cast<char*>(buffer), 4);
    if (is) {
      data = buffer[0];
      data <<= 8;
      data |= buffer[1];
      data <<= 8;
      data |= buffer[2];
      data <<= 8;
      data |= buffer[3];
    }
    return is;
  }

  inline
  std::istream &read_uint64(std::istream &is, uint64_t &data)
  {
    unsigned char buffer[8];
    is.read(reinterpret_cast<char*>(buffer), 8);
    if (is) {
      data = buffer[0];
      data <<= 8;
      data |= buffer[1];
      data <<= 8;
      data |= buffer[2];
      data <<= 8;
      data |= buffer[3];
      data <<= 8;
      data |= buffer[4];
      data <<= 8;
      data |= buffer[5];
      data <<= 8;
      data |= buffer[6];
      data <<= 8;
      data |= buffer[7];
    }
    return is;
  }

  //////////////////////////////////////////////////////////////////////////////

  template <typename T>
  std::istream &read_type_n(std::istream &is, std::size_t size,
      T &data, uint8_t &remainder) {
    const std::size_t read_size = (size + 7) >> 3 << 3;

    if (size == read_size) {
      unsigned char buffer[read_size >> 3];
      is.read(reinterpret_cast<char*>(buffer), read_size >> 3);
      if (is) {
        data = 0;
        for (std::size_t i = 0; i != read_size >> 3; data |= buffer[i++])
          data <<= 8;
      }
    } else {
      T buffer;
      //std::cout << __FILE__ << __LINE__ << std::endl;
      if (read_type_n(is, read_size, buffer, remainder)) {
        remainder = buffer & (1 << (read_size - size)) - 1;
        data = buffer >> (read_size - size);
      }
    }

    return is;
  }

  template <std::size_t size, typename T>
  std::istream &read_type_n(std::istream &is, T &data, uint8_t &remainder) {
    constexpr std::size_t read_size = (size + 7) >> 3 << 3;
    if (size == read_size) {
      unsigned char buffer[read_size >> 3];
      is.read(reinterpret_cast<char*>(buffer), read_size >> 3);
      if (is) {
        data = 0;
        for (std::size_t i = 0; i != read_size >> 3; data |= buffer[i++])
          data <<= 8;
      }
    } else {
      T buffer;
      //std::cout << __FILE__ << __LINE__ << std::endl;
      if (read_type_n<read_size>(is, buffer, remainder)) {
        remainder = buffer & (1 << (read_size - size)) - 1;
        data = buffer >> (read_size - size);
      }
    }
    return is;
  }

  template <>
  std::istream &read_type_n<8>(std::istream &, char &, uint8_t &);
  template <>
  std::istream &read_type_n<8>(std::istream &, uint8_t &, uint8_t &);
  template <>
  std::istream &read_type_n<16>(std::istream &, uint16_t &, uint8_t &);
  template <>
  std::istream &read_type_n<24>(std::istream &, uint32_t &, uint8_t &);
  template <>
  std::istream &read_type_n<32>(std::istream &, uint32_t &, uint8_t &);
  template <>
  std::istream &read_type_n<64>(std::istream &, uint64_t &, uint8_t &);

  template <std::size_t size, std::size_t remainder_digit, typename T>
  std::istream &read_type_n_with_remainder(std::istream &is, T &data, 
      uint8_t &remainder)
  {
    T buffer = remainder << (size - remainder_digit);
    T tmp;
    if (read_type_n<size - remainder_digit>(is, tmp, remainder))
      data = buffer | tmp;

    return is;
  }

  template <typename T>
  std::istream &read_type_n_with_remainder(std::istream &is, 
      std::size_t size, T &data, 
      uint8_t &remainder, unsigned &remainder_digit)
  {
    T buffer = remainder << (size -= remainder_digit);
    T tmp;
    if (read_type_n(is, size, tmp, remainder))
      data = buffer | tmp;

    //remainder_digit = (size + 7) & ~7 - size;
    remainder_digit = ((size ^ 7) + 1 & 7);

    return is;
  }

  // remainder_digit < 8
  template <typename T>
  char *package(char *buffer, std::size_t size, T data, 
      uint8_t &remainder, unsigned &remainder_digit) {

    std::size_t tot_size = (remainder_digit + size);
    if (tot_size < 8) {
      remainder <<= size;
      remainder |= (data & ((1 << size) - 1));
      remainder_digit += size;
      return buffer;
    }

    unsigned new_digit = tot_size & 7;
    std::size_t write_byte = tot_size >> 3; // positive number

    unsigned new_remainder = data & ((1 << new_digit) - 1);
    data >>= new_digit;

    auto ubuffer = reinterpret_cast<uint8_t *>(buffer);
    for (auto ptr = ubuffer + write_byte; ptr != ubuffer; ) {
      *--ptr = data & 0xff;
      data >>= 8;
    }

    *ubuffer &= (1 << (8 - remainder_digit)) - 1;
    *ubuffer |= remainder << (8 - remainder_digit);

    // SUCCESS
    remainder = new_remainder;
    remainder_digit = new_digit;
    return buffer + write_byte;
  }

  template <std::size_t size, typename T>
  char *package(char *buffer, T data, 
      uint8_t &remainder, unsigned &remainder_digit) {
    return package(buffer, size, data, remainder, remainder_digit);
  }
}
#endif // __FLAC__FLAC_UTILITIES_H_INCLUDED
