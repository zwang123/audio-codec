#ifndef __WAV__WAV_H_INCLUDED
#define __WAV__WAV_H_INCLUDED

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename DataType>
class WAV_PCM {
public:
  constexpr static int RETURN_SUCCESS = 0;
  constexpr static int FILE_ERROR = 1 << 15;

  WAV_PCM() = default;
  explicit WAV_PCM(const char *filename) {
    auto rtn = read(filename);
    if (rtn != RETURN_SUCCESS)
      throw std::runtime_error(
          std::string("Failed to open file ") + filename +
          ". Error code: " + std::to_string(rtn));
  }

  int read(const char *filename) {
    std::ifstream file(filename); 
    //, std::ifstream::in);// | std::ifstream::binary);
    if (!file)
      return FILE_ERROR;
    // TODO exception free, should build a temp obj and copy to the current
    file.read(chunkID, 4);
    read_type(file, chunkSize);
    file.read(format, 4);
    file.read(fmtSubChunkID, 4);
    read_type(file, fmtSubChunkSize);
    read_type(file, audioFormat);
    read_type(file, numChannels);
    read_type(file, sampleRate);
    read_type(file, byteRate);
    read_type(file, blockAlign);
    read_type(file, bitsPerSample);

    CHUNK_SIZE_OFFSET = DEFAULT_CHUNK_SIZE_OFFSET;
    while (true) {
      char subChunkID[5] = {0};
      file.read(subChunkID, 4);
      if (strncmp(subChunkID, "data", 4) == 0)
        break;

      uint32_t subChunkSize;
      read_type(file, subChunkSize);

      std::string tmp(subChunkSize, 0);
      // compatibility
      file.read(&tmp[0], subChunkSize);
      std::cout << subChunkID << std::endl;
      std::cout << tmp << std::endl;
      std::cout << subChunkID << std::endl;
      CHUNK_SIZE_OFFSET += subChunkSize + 8;
    }

    strncpy(dataSubChunkID, "data", 4);
    read_type(file, dataSubChunkSize);

    data.resize(numChannels);
    for (auto &channel : data) {
      channel.clear();
      channel.reserve(dataSubChunkSize / blockAlign);
    }

    uint32_t data_flatten_size = dataSubChunkSize / sizeof(DataType);

    for (uint32_t i = 0; i != data_flatten_size; ++i) {
      DataType tmp;
      read_type(file, tmp);
      data[i % numChannels].push_back(tmp);
    }

    file.close();
    return check();
  }

  int write(const char *filename) {
    CHUNK_SIZE_OFFSET = DEFAULT_CHUNK_SIZE_OFFSET;
    chunkSize = CHUNK_SIZE_OFFSET + dataSubChunkSize;

    auto chk = check();
    if (chk != RETURN_SUCCESS)
      return chk;

    std::ofstream file(filename); 
    if (!file)
      return FILE_ERROR;
    file.write(chunkID, 4);
    write_type(file, chunkSize);
    file.write(format, 4);
    file.write(fmtSubChunkID, 4);
    write_type(file, fmtSubChunkSize);
    write_type(file, audioFormat);
    write_type(file, numChannels);
    write_type(file, sampleRate);
    write_type(file, byteRate);
    write_type(file, blockAlign);
    write_type(file, bitsPerSample);
    file.write(dataSubChunkID, 4);
    write_type(file, dataSubChunkSize);

    if (!data.empty()) {
      uint32_t nSamples = data.front().size();

      for (uint32_t i = 0; i != nSamples; ++i) {
        for (uint32_t j = 0; j != numChannels; ++j) {
          write_type(file, data[j][i]);
        }
      }
    }

    //std::cout << __LINE__ << std::endl;
    file.close();
    return RETURN_SUCCESS;
  }

  int check() const {
    int rtn = RETURN_SUCCESS;
    if (strncmp(chunkID, "RIFF", 4) != 0)
      rtn |= 1 << 0;
    if (chunkSize != dataSubChunkSize + CHUNK_SIZE_OFFSET) {
      std::cout << "dataSubChunkSize: " << dataSubChunkSize << std::endl;
      std::cout << "chunkSize: " << chunkSize << std::endl;
      rtn |= 1 << 1;
    }
    if (strncmp(format, "WAVE", 4) != 0)
      rtn |= 1 << 2;
    if (strncmp(fmtSubChunkID, "fmt ", 4) != 0)
      rtn |= 1 << 3;
    if (fmtSubChunkSize != 16)
      rtn |= 1 << 4;
    if (audioFormat != 1)
      rtn |= 1 << 5;
    if (byteRate * BITS_PER_BYTE != sampleRate * bitsPerSample * numChannels)
      rtn |= 1 << 6;
    if (blockAlign * BITS_PER_BYTE != bitsPerSample * numChannels)
      rtn |= 1 << 7;
    if (bitsPerSample != sizeof(DataType) * BITS_PER_BYTE)
      rtn |= 1 << 8;
    if (strncmp(dataSubChunkID, "data", 4) != 0) {
      std::cout << dataSubChunkID[0];
      std::cout << dataSubChunkID[1];
      std::cout << dataSubChunkID[2];
      std::cout << dataSubChunkID[3];
      std::cout << std::endl;
      rtn |= 1 << 9;
    }
    if (numChannels != data.size())
      rtn |= 1 << 10;
    int err = 1 << 11;
    for (const auto &channel : data) {
      if (dataSubChunkSize != channel.size() * blockAlign)
        rtn |= err;
      err <<= 1;
    }
    return rtn;
  }

  friend bool operator==(const WAV_PCM &lhs, const WAV_PCM &rhs)
  {
    return lhs.CHUNK_SIZE_OFFSET == rhs.CHUNK_SIZE_OFFSET &&
      (strncmp(lhs.chunkID, rhs.chunkID, 4) == 0) &&
      lhs.chunkSize == rhs.chunkSize &&
      (strncmp(lhs.format, rhs.format, 4) == 0) &&
      (strncmp(lhs.fmtSubChunkID, rhs.fmtSubChunkID, 4) == 0) &&
      lhs.fmtSubChunkSize == rhs.fmtSubChunkSize &&
      lhs.audioFormat == rhs.audioFormat &&
      lhs.numChannels == rhs.numChannels &&
      lhs.sampleRate == rhs.sampleRate &&
      lhs.byteRate == rhs.byteRate &&
      lhs.blockAlign == rhs.blockAlign &&
      lhs.bitsPerSample == rhs.bitsPerSample &&
      (strncmp(lhs.dataSubChunkID, rhs.dataSubChunkID, 4) == 0) &&
      lhs.dataSubChunkSize == rhs.dataSubChunkSize &&
      lhs.data == rhs.data;
  }
private:
  constexpr static uint32_t DEFAULT_CHUNK_SIZE_OFFSET = 36;
  uint32_t CHUNK_SIZE_OFFSET = DEFAULT_CHUNK_SIZE_OFFSET;
  constexpr static uint8_t BITS_PER_BYTE = 8;

  // RIFF chunk descriptor
  char chunkID[4] = {'R', 'I', 'F', 'F'};
  uint32_t chunkSize = CHUNK_SIZE_OFFSET;
  char format[4] = {'W', 'A', 'V', 'E'};

  // fmt sub-chunk
  char fmtSubChunkID[4] = {'f', 'm', 't', ' '};
  uint32_t fmtSubChunkSize = 16;
  uint16_t audioFormat = 1; // 1 for PCM
  uint16_t numChannels = 0; // 1 for mono, 2 for stereo
  uint32_t sampleRate = 44100; // 44100 for CD, 48000 for DAT
  uint32_t byteRate = 0; //  (sampleRate * bitsPerSample * numChannels) / 8
  uint16_t blockAlign = 0; // (bitsPerSample * numChannels) / 8
  uint16_t bitsPerSample = 16; // bit depth

  // data sub-chunk
  char dataSubChunkID[4] = {'d', 'a', 't', 'a'};
  uint32_t dataSubChunkSize = 0;
  std::vector<std::vector<DataType>> data;

  template <typename T>
  static std::istream &read_type(std::istream &is, T &data) {
    is.read(reinterpret_cast<char*>(&data), sizeof(data));
    return is;
  }

  template <typename T>
  static std::ostream &write_type(std::ostream &os, T &data) {
    os.write(reinterpret_cast<char*>(&data), sizeof(data));
    return os;
  }

};

extern template class WAV_PCM<uint8_t>;
using WAV_PCMU8 = WAV_PCM<uint8_t>;
extern template class WAV_PCM<int16_t>;
using WAV_PCMS16 = WAV_PCM<int16_t>;
extern template class WAV_PCM<int32_t>;
using WAV_PCMS32 = WAV_PCM<int32_t>;

//extern template bool operator==<>(const WAV_PCMU8 &, const WAV_PCMU8 &);
//extern template bool operator==<>(const WAV_PCMS16 &, const WAV_PCMS16 &);
//extern template bool operator==<>(const WAV_PCMS32 &, const WAV_PCMS32 &);

#endif // __WAV__WAV_H_INCLUDED
