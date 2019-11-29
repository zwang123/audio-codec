#include <istream>
//#include <stdexcept>

namespace flac {
class istream_iterator {
public:
  // this is not an input iterator !
  using value_type = char;
  using pointer = const value_type*;
  using reference = const value_type&;
  using istream_type = std::istream;

  // user should make sure there is only 1 istream_iterator with unread flag
  // associated with  each istream
  istream_iterator(istream_type &stream)
    : is(stream), buffer(0), read_flag(false) {}
  istream_iterator(const istream_iterator &) = default;
  ~istream_iterator() = default;

  // user should make sure read_flag is true
  reference operator*() const {
    return buffer;
  }
  pointer operator->() const { return &operator*(); }

  // do not copy after ++*this;
  istream_iterator &operator++() {
    if (read_flag) read_flag = false;
    else is.get(buffer);
    return *this;
  }

  // expect to be dereferenced soon
  istream_iterator operator++(int) {
    auto rtn = operator++();
    rtn.read_flag = true;
    return rtn;
  }

private:
  istream_type &is;
  value_type buffer;
  bool read_flag;
};
}
