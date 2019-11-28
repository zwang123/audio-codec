#ifndef __FLAC__FLAC_RESIDUAL_H_INCLUDED
#define __FLAC__FLAC_RESIDUAL_H_INCLUDED

#include <cstdint>
#include <iostream>
#include <vector>
#include "FlacRicePartition.h"

#include "FlacErrorCodes.h"
#include "FlacUtilities.h"
#include "crc.h"

namespace flac {
class FlacResidual {
public:
  FlacResidual(uint16_t _blocksize, uint_fast8_t _predictor_order) 
    : blocksize(_blocksize), predictor_order(_predictor_order)
  {}

  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
  {
    read_type_n_with_remainder(is, 2, rice_parameter_bit_size, 
          remainder, remainder_digit); 
    rice_parameter_bit_size += 4;
    read_type_n_with_remainder(is, 4, partition_order, 
          remainder, remainder_digit); 

    rice_partitions.clear();
    uint16_t partition_size = 1 << (partition_order &= 0xf);
    rice_partitions.emplace_back(is, remainder, remainder_digit,
        rice_parameter_bit_size, 
        (blocksize >> partition_order) - predictor_order);
    while (--partition_size) {
      rice_partitions.emplace_back(is, remainder, remainder_digit,
          rice_parameter_bit_size, blocksize >> partition_order);
    }
    return RETURN_SUCCESS;
  }

  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const
  {
    char buffer[2];
    char *ptr = buffer;
    ptr = package(ptr, 2, rice_parameter_bit_size - 4, 
        remainder, remainder_digit);
    ptr = package(ptr, 4, partition_order, remainder, remainder_digit);
    os.write(buffer, ptr - buffer);
    crc16_encodes(buffer, ptr, crc16);
    for (const auto &partition : rice_partitions)
      partition.write(os, remainder, remainder_digit, crc16);
    return RETURN_SUCCESS;
  }

private:
  uint16_t blocksize;
  uint_fast8_t predictor_order;
  uint_fast8_t rice_parameter_bit_size;
  uint8_t partition_order;
  std::vector<FlacRicePartition> rice_partitions;
};
}
#endif // __FLAC__FLAC_RESIDUAL_H_INCLUDED
