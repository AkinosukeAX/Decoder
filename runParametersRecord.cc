#include "runParametersRecord.hh"

runParametersRecord::runParametersRecord() : algoBase() {}

void runParametersRecord::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile)
{
  uint32_t bufInt;
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_marker = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_record_size = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_run_number = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_max_events = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_rec_enable = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_trigger_type = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_detector_mask_1of2 = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_detector_mask_2of2 = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_beam_type = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_beam_energy = bufInt;
  
  // this is not documented properly but there are two more words
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
}

runParametersRecord::~runParametersRecord() {}
