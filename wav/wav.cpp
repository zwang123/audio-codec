#include "wav.h"
template class WAV_PCM<uint8_t>;
template class WAV_PCM<int16_t>;
template class WAV_PCM<int32_t>;

//template bool operator==<>(const WAV_PCMU8 &, const WAV_PCMU8 &);
//template bool operator==<>(const WAV_PCMS16 &, const WAV_PCMS16 &);
//template bool operator==<>(const WAV_PCMS32 &, const WAV_PCMS32 &);
