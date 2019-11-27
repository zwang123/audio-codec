#ifndef __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
#include "FlacSubframeData.h"

namespace flac {
template <typename DataType>
class FlacSubframeConstant : public FlacSubframeData {
  DataType value = 0;
};
}
#endif // __FLAC__FLAC_SUBFRAME_CONSTANT_H_INCLUDED
