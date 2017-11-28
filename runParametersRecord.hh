#ifndef runParametersRecord_hh
#define runParametersRecord_hh

#include <fstream>
#include <stdint.h>
#include "algoBase.hh"

class runParametersRecord : public algoBase {
public:
  runParametersRecord();
  ~runParametersRecord();

private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile);

  uint32_t m_marker;
  uint32_t m_record_size;
  uint32_t m_run_number;
  uint32_t m_max_events;
  uint32_t m_rec_enable;
  uint32_t m_trigger_type;
  uint32_t m_detector_mask_1of2;
  uint32_t m_detector_mask_2of2;
  uint32_t m_beam_type;
  uint32_t m_beam_energy;
};

#endif
