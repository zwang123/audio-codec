#ifndef __FLAC__FLAC_RICE_PARTITION_H_INCLUDED
#define __FLAC__FLAC_RICE_PARTITION_H_INCLUDED

#include <cstdint>
#include <iostream>

#include "FlacErrorCodes.h"
#include "FlacUtilities.h"
#include "crc.h"
#include "UnaryCodec.h"

namespace flac {
class FlacRicePartition {
using DataType = uint32_t;
using value_type = std::valarray<DataType>; // ctor diff from vector
public:
  FlacRicePartition(
      std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint_fast8_t _rice_parameter_bit_size, uint16_t _partition_size) 
    : rice_parameter_bit_size(_rice_parameter_bit_size)
    , residuals(_partition_size)
  {read(is, remainder, remainder_digit);}

  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
  {
    read_type_n_with_remainder(is, rice_parameter_bit_size, rice_parameter, 
          remainder, remainder_digit); 

    std::cout << "rice_parameter: " << (unsigned)rice_parameter << std::endl;

    if (rice_parameter != (1 << rice_parameter_bit_size) - 1) {

      //uint_fast16_t buffer;
      //int bit_counter = 0;
      //for (int i = 0; i != 356; ++i) {
      //  read_type_n_with_remainder(is, 16, buffer, 
      //        remainder, remainder_digit); 
      //  //std::cout << buffer << " ";
      //  //if (bit_counter > 4084) {
      //  //  std::cout << bit_counter << " " << buffer << " ";
      //  //}
      //  while (buffer) {
      //    if (buffer & 1) {
      //      ++bit_counter;
      //    }
      //    buffer >>= 1;
      //  }
      //  if (bit_counter > 4088) {
      //    std::cout << bit_counter << " " << buffer << " ";
      //    break;
      //  }
      //}
      //std::cout << std::endl;
      //  read_type_n_with_remainder(is, 11, buffer, 
      //        remainder, remainder_digit); 
      //  //std::cout << buffer << " ";
      //  while (buffer) {
      //    if (buffer & 1) {
      //      ++bit_counter;
      //    }
      //    buffer >>= 1;
      //  }
      //    std::cout << bit_counter << " " << buffer << " ";

      //for (int i = 0; i != 16; ++i) {
      //  read_type_n_with_remainder(is, 16, buffer, 
      //        remainder, remainder_digit); 
      //  //std::cout << buffer << " ";
      //  //if (bit_counter > 4084) {
      //  //  std::cout << bit_counter << " " << buffer << " ";
      //  //}
      //    std::cout << bit_counter << " " << buffer << " ";
      //  while (buffer) {
      //    if (buffer & 1) {
      //      ++bit_counter;
      //    }
      //    buffer >>= 1;
      //  }
      //}
      //assert(0);

      for (auto &x : residuals) {
        DataType head = 0;
        unary_decode(is, head, remainder, remainder_digit);
        DataType tail = 0;
    //std::cout << __FILE__ << __LINE__ << std::endl;
        read_type_n_with_remainder(is, rice_parameter, tail, 
              remainder, remainder_digit); 
    //std::cout << __FILE__ << __LINE__ << std::endl;
        uint32_t tmp = head << rice_parameter | tail;
        if (tmp & 1)
          x = -static_cast<DataType>(tmp >> 1) - 1;
        else
          x = static_cast<DataType>(tmp >> 1);
      }
    } else {
      assert(0);
      uint8_t bits_per_sample;
      read_type_n_with_remainder(is, 5, bits_per_sample, 
            remainder, remainder_digit); 
      for (auto &x : residuals) {
        read_type_n_with_remainder(is, bits_per_sample, x,
              remainder, remainder_digit); 
      }
    }

    //int i = 1579;
    //std::cout << i << " " << residuals[i] << std::endl;

    //std::cout << __FILE__ << __LINE__ << std::endl;
    //std::cout << "_partsize: " << residuals.size() << std::endl;
    //assert(0);
    return RETURN_SUCCESS;
  }

  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const {return 0;}
private:
  uint_fast8_t rice_parameter_bit_size;
  uint_fast8_t rice_parameter;
  value_type   residuals;
};
}
#endif // __FLAC__FLAC_RICE_PARTITION_H_INCLUDED
