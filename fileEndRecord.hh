#ifndef fileEndRecord_hh
#define fileEndRecord_hh

#include <fstream>
#include <stdint.h>

#include "algoBase.hh"

class fileEndRecord : public algoBase {
public:
  fileEndRecord();
  ~fileEndRecord();
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile);

  uint32_t m_marker;
  uint32_t m_record_size;
  uint32_t m_data;
  uint32_t m_time;
  uint32_t m_events_in_file;
  uint32_t m_data_in_file;
  uint32_t m_events_in_run;
  uint32_t m_data_in_run;
  uint32_t m_status;
  uint32_t m_end_marker;
};

#endif
