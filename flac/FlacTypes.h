#ifndef __FLAC__FLAC_TYPES_H_INCLUDED
#define __FLAC__FLAC_TYPES_H_INCLUDED
#include <iostream>
#include <fstream>

namespace flac {
  using uistream = std::basic_istream<unsigned char>;
  using uostream = std::basic_ostream<unsigned char>;
  using uifstream = std::basic_ifstream<unsigned char>;
  using uofstream = std::basic_ofstream<unsigned char>;
}

#endif // __FLAC__FLAC_TYPES_H_INCLUDED
