#ifndef fileNameStrings_hh
#define fileNameStrings_hh

#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>

#include "algoBase.hh"

class fileNameStrings : public algoBase {
public:
  fileNameStrings();
  ~fileNameStrings();
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile);

  uint32_t m_marker;
  std::string m_appName;
  std::string m_fileNameCore;
};

#endif 
