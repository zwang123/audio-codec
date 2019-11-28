#include "../utf8.h"
#include <iostream>


static char buffer[7];

void test_encode(uint64_t input)
{
  char *end = flac::utf8_encode(input, buffer);
  std::cout << input << std::endl;
  for (char *p = buffer; p != end;)
    std::cout << (unsigned)static_cast<unsigned char>(*p++) << " ";
  std::cout << std::endl;
  std::cout << flac::utf8_decode(buffer);
  std::cout << std::endl;
  std::cout << std::endl;
}

int main()
{
  std::cout << std::hex;
  test_encode(0);
  test_encode(0x7f);

  test_encode(0x80);
  test_encode(0x7ff);

  test_encode(0x800);
  test_encode(0xffff);

  test_encode(0x10000);
  test_encode(0x1fffff);

  test_encode(0x200000);
  test_encode(0x3ffffff);

  test_encode(0x4000000);
  test_encode(0x7fffffff);

  test_encode(0x80000000);
  test_encode(0xfffffffff);
  return 0;
}
