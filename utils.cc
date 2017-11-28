#include "utils.hh"

uint32_t utils::getWordLength(uint32_t charLength){
  return (charLength-charLength%4)/4 + ((charLength%4)==0 ? 0 : 1);
}

std::string utils::getResult48bits(uint64_t value) {
  std::string rc;
  for (int ii=0; ii<48; ii++) {
    if (ii!=0 && ii%4==0) { rc += "_"; }
    rc += (((value >> 47-ii) & 0X1)==0X1) ? "1" : "0";
  }
  return rc;
}

std::string utils::get64bits(uint64_t value, int nbits) {
  std::string rc;
  for (int ii=0; ii<nbits; ii++) {
    rc += (((value >> nbits-1-ii) & 0X1)==0X1) ? "1" : "0";
  }
  return rc;
}

std::string utils::get32bits(uint32_t value, int nbits) {
  std::string rc;
  for (int ii=0; ii<nbits; ii++) {
    rc += (((value >> nbits-1-ii) & 0X1)==0X1) ? "1" : "0";
  }
  return rc;
}
