#include "metadataStrings.hh"
#include "utils.hh"

metadataStrings::metadataStrings() : algoBase() {}

void
metadataStrings::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile)
{
  uint32_t bufInt;
  char buf;
  uint32_t wordLength;
  uint32_t length;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_marker = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_numberOfFreeMetadataStrings = bufInt;
  
  // guid
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  wordLength = utils::getWordLength(bufInt);
  length = bufInt;
  for (uint32_t ii=0; ii<wordLength; ii++) {
    for (uint32_t iChar=0; iChar<4; iChar++) {
      inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
      if (ii*4+iChar<length) { m_guid.append(1, buf); }
    }
  }
  //printf("m_guid=%s\n", m_guid.c_str());

  
  // // metadata_string
  // inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  // wordLength = utils::getWordLength(bufInt);
  // length = bufInt;
  // for (uint32_t ii=0; ii<wordLength; ii++) {
  //   for (uint32_t iChar=0; iChar<4; iChar++) {
  //     inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
  //     if (ii*4+iChar<length) { m_metadata_string.append(1, buf); }
  //   }
  // }
  // printf("m_metadata_string=%s\n", m_metadata_string.c_str());

  
  // streamTag
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  wordLength = utils::getWordLength(bufInt);
  length = bufInt;
  for (uint32_t ii=0; ii<wordLength; ii++) {
    for (uint32_t iChar=0; iChar<4; iChar++) {
      inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
      if (ii*4+iChar<length) { m_streamTag.append(1, buf); }
    }
  }
  //printf("m_streamTag=%s\n", m_streamTag.c_str());
  
  
  // projectTag
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  wordLength = utils::getWordLength(bufInt);
  length = bufInt;
  for (uint32_t ii=0; ii<wordLength; ii++) {
    for (uint32_t iChar=0; iChar<4; iChar++) {
      inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
      if (ii*4+iChar<length) { m_projectTag.append(1, buf); }
    }
  }
  //printf("m_projectTag=%s\n", m_projectTag.c_str());
  
  
  // LuminosityBlockNumber
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  wordLength = utils::getWordLength(bufInt);
  length = bufInt;
  for (uint32_t ii=0; ii<wordLength; ii++) {
    for (uint32_t iChar=0; iChar<4; iChar++) {
      inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
      if (ii*4+iChar<length) { m_LuminosityBlockNumber.append(1, buf); }
    }
  }
  //printf("m_LuminosityBlockNumber=%s\n", m_LuminosityBlockNumber.c_str());
  
  
  // // compression
  // inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  // wordLength = utils::getWordLength(bufInt);
  // length = bufInt;
  // for (uint32_t ii=0; ii<wordLength; ii++) {
  //   for (uint32_t iChar=0; iChar<4; iChar++) {
  //     inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
  //     if (ii*4+iChar<length) { m_compression.append(1, buf); }
  //   }
  // }
  // printf("m_compression=%s\n", m_compression.c_str());
}

metadataStrings::~metadataStrings() {}
