#ifndef __FLAC__FLAC_ERROR_CODES_H_INCLUDED
#define __FLAC__FLAC_ERROR_CODES_H_INCLUDED
namespace flac {
  constexpr static int RETURN_SUCCESS = 0;
  constexpr static int FILE_ERROR = 1 << 15;
  constexpr static int STREAM_ERROR = 1 << 14;
}
#endif // __FLAC__FLAC_ERROR_CODES_H_INCLUDED
