#ifndef metadataStrings_hh
#define metadataStrings_hh

#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>

#include "algoBase.hh"

class metadataStrings : public algoBase
{
public:
  metadataStrings();
  ~metadataStrings();
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile);

  uint32_t m_marker;
  uint32_t m_numberOfFreeMetadataStrings;
  std::string m_guid;
  std::string m_metadata_string;
  std::string m_streamTag;
  std::string m_projectTag;
  std::string m_LuminosityBlockNumber;
  std::string m_compression;
};

#endif

