#ifndef __FLAC__FLAC_SUBFRAME_DATA_H_INCLUDED
#define __FLAC__FLAC_SUBFRAME_DATA_H_INCLUDED

namespace flac {
class FlacSubframeData {
public:
  //virtual int write(std::ostream &, uint8_t &, unsigned &) const = 0;
  virtual int write(std::ostream &, uint8_t &, unsigned &, 
      uint16_t &) const = 0;
  virtual ~FlacSubframeData() {}
protected:
  FlacSubframeData() = default;
};
}
#endif // __FLAC__FLAC_SUBFRAME_DATA_H_INCLUDED
