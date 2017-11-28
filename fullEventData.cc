#include "fullEventData.hh"

fullEventData::fullEventData() : algoBase() {}

void
fullEventData::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile)
{
  uint32_t bufInt;
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_marker = bufInt;
  //printf("m_marker=%08x\n", m_marker);
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_total_fragment_size = bufInt;
  
  for (uint32_t iWord=2; iWord<m_total_fragment_size; iWord++) {
    std::ifstream::pos_type currp = inputfile.tellg();
    inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
    //printf("fullEventData bufInt=%08x iWord=%u m_total_fragment_size=%u\n", bufInt, iWord, m_total_fragment_size);
    
    if (bufInt==0XDD1234DD) { // ROB format
      inputfile.clear();
      inputfile.seekg(currp);
      iWord--;
      robdata rob;
      rob.SetDebug(m_debug); rob.SetName("Readout Buffer Data");
      rob.ProceedFilePointerAndDecode(inputfile, iWord);
      m_robdata.push_back(rob);
    }
  }
}

fullEventData::~fullEventData() {}
