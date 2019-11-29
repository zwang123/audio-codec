#include <istream>
//#include <stdexcept>

namespace flac {
template <class T>
class istream_iterator {
public:
  // this is not an input iterator !
  using value_type = T;
  using char_type = char;
  using pointer = const T*;
  using reference = const T&;
  using istream_type = std::istream;

  // user should make sure there is only 1 istream_iterator with unread flag
  // associated with  each istream
  istream_iterator(istream_type &stream)
    : is(stream), buffer(0), read_flag(false) {}
  istream_iterator(const istream_iterator &) = default;

  //istream_iterator(const istream_iterator &other)
  //  : is(other.is), buffer(other.buffer), read_flag(other.read_flag)
  //{ if (!other.read_flag) throw std::runtime_error(
  //    "not allowed to copy unread istream_iterator"); }

  ~istream_iterator() = default;

  // only read the istream at dereference
  reference operator*() {
    if (read_flag) return buffer;
    is.get(buffer);
    read_flag = true;
    return buffer;
  }
  pointer operator->() { return &operator*(); }

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
  char_type buffer;
  bool read_flag;
};
}
