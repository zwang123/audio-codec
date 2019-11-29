#ifndef __FLAC__ISTREAM_ITERATOR_H_INCLUDED
#define __FLAC__ISTREAM_ITERATOR_H_INCLUDED
#include <istream>

namespace flac {

// the only usage: *iterator++

class istream_iterator {
public:
  // this is not an input iterator !
  using value_type = char;
  using reference = const value_type&;
  using istream_type = std::istream;

  // user should make sure there is only 1 istream_iterator with unread flag
  // associated with  each istream
  istream_iterator(istream_type &stream)
    : is(stream), buffer(0) {}
  istream_iterator(const istream_iterator &) = default;
  ~istream_iterator() = default;

  reference operator*() const {
    return buffer;
  }

  // expect to be dereferenced soon
  istream_iterator operator++(int) {
    is.get(buffer);
    return *this;
  }

private:
  istream_type &is;
  value_type buffer; // store the prev value
  bool read_flag;
};
}
#endif // __FLAC__ISTREAM_ITERATOR_H_INCLUDED
