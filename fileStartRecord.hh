// https://edms.cern.ch/ui/file/580290/6/DC-066.pdf

#ifndef fileStartRecord_hh
#define fileStartRecord_hh

#include <fstream>
#include <stdint.h>

#include "algoBase.hh"

class fileStartRecord : public algoBase
{
public:
  fileStartRecord();
  ~fileStartRecord();
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile);

  uint32_t m_marker;
  uint32_t m_record_size;
  uint32_t m_version;
  uint32_t m_file_number;
  uint32_t m_date;
  uint32_t m_time;
  uint32_t m_sizeLimit_dataBlocks;
  uint32_t m_sizeLimit_MB;
};

#endif
