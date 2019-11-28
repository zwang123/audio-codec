#ifndef __FLAC__FLAC_SUBFRAME_LPC_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_LPC_H_INCLUDED

#include <iostream>
#include <valarray>
#include "FlacResidual.h"
#include "FlacSubframeData.h"

#include "FlacErrorCodes.h"
#include "FlacUtilities.h"
#include "crc.h"

namespace flac {
class FlacSubframeLpc : public FlacSubframeData {
using DataType = uint32_t;
using value_type = std::valarray<DataType>; // ctor diff from vector
public:
  FlacSubframeLpc(
      std::istream &is, uint8_t &remainder, unsigned &remainder_digit,
      uint8_t _bits_per_sample, uint16_t _blocksize, uint_fast8_t _order)
    : bits_per_sample(_bits_per_sample)
    , warm_up_samples(_order)
    , qlp_coeffs(_order)
    , residual(_blocksize, _order) 
  { read(is, remainder, remainder_digit); }

  int read(std::istream &is, uint8_t &remainder, unsigned &remainder_digit)
      override
  { 
    for (auto &value : warm_up_samples)
      read_type_n_with_remainder(is, bits_per_sample, value, 
            remainder, remainder_digit); 
    read_type_n_with_remainder(is, 4, qlp_coeff_precision, 
          remainder, remainder_digit); 
    ++qlp_coeff_precision;
    read_type_n_with_remainder(is, 5, qlp_coeff_shift, 
          remainder, remainder_digit); 
    if (qlp_coeff_shift & 0x10) {
      qlp_coeff_shift |= ~static_cast<decltype(qlp_coeff_shift)>(0xf);
    }
    for (auto &value : qlp_coeffs)
      read_type_n_with_remainder(is, qlp_coeff_precision, value, 
            remainder, remainder_digit); 
    residual.read(is, remainder, remainder_digit);
    return RETURN_SUCCESS;
  }
  int write(std::ostream &os, uint8_t &remainder, unsigned &remainder_digit, 
      uint16_t &crc16) const override
  {
    char buffer[MAX_LPC_HEADER_SIZE];
    char *ptr = buffer;
    for (const auto value : warm_up_samples)
      ptr = package(ptr, bits_per_sample, value, remainder, remainder_digit);
    ptr = package(ptr, 4, qlp_coeff_precision - 1, remainder, remainder_digit);
    ptr = package(ptr, 5, qlp_coeff_shift, remainder, remainder_digit);
    for (const auto value : qlp_coeffs)
      ptr = package(ptr, qlp_coeff_precision, value, 
          remainder, remainder_digit);
    os.write(buffer, ptr - buffer);
    crc16_encodes(buffer, ptr, crc16);
    residual.write(os, remainder, remainder_digit, crc16);
    return RETURN_SUCCESS;
  }
private:
  constexpr static uint32_t MAX_LPC_HEADER_SIZE = 200;
  uint8_t bits_per_sample;
  value_type warm_up_samples;
  uint_fast8_t qlp_coeff_precision;
  int_fast8_t qlp_coeff_shift; // two's complement
  value_type qlp_coeffs; // two's complement
  FlacResidual residual;
};
}
#endif // __FLAC__FLAC_SUBFRAME_LPC_H_INCLUDED
