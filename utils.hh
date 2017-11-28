#ifndef __utils__hh
#define __utils__hh

#include <stdint.h>
#include <string>

namespace utils {
  uint32_t getWordLength(uint32_t charLength);
  std::string get64bits(uint64_t value, int nbits);
  std::string get32bits(uint32_t value, int nbits);
  std::string getResult48bits(uint64_t value);
}

#endif

