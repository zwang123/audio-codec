#include <cassert>
//#include <cstdint>
#include <iostream>
#include <stdexcept>
#include "wav.h"

//using WAV_PCMS16 = WAV_PCM<int16_t>;

int main()
{
  WAV_PCMS16 wav;

  auto rtn = wav.check();
  std::cout << rtn << std::endl;
  assert(rtn == WAV_PCMS16::RETURN_SUCCESS);

  rtn = wav.read("xxx.wav");
  std::cout << rtn << std::endl;
  assert(rtn == WAV_PCMS16::RETURN_SUCCESS);

  //auto rtn = wav.check(); check is auto run
  //std::cout << std::hex << wav.check() << std::endl;
  //assert(wav.check() == WAV_PCMS16::RETURN_SUCCESS);
  
  rtn = wav.write("yyy.wav");
  std::cout << rtn << std::endl;
  assert(rtn == WAV_PCMS16::RETURN_SUCCESS);

  try {
    WAV_PCMU8 tmp("yyy.wav");
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  try {
    WAV_PCMS16 tmp("zzz.wav");
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  WAV_PCMS16 wav2("yyy.wav");
  assert(rtn == WAV_PCMS16::RETURN_SUCCESS);
  assert(wav == wav2);
  return 0;
}

