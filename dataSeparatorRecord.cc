#include "dataSeparatorRecord.hh"

dataSeparatorRecord::dataSeparatorRecord() : algoBase() {}

void dataSeparatorRecord::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile)
{
  uint32_t bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_marker = bufInt;
  //printf("m_marker=%08x\n", m_marker);
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_record_size = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_data_block_number = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_block_size = bufInt;
  
  //printf("block number=%u size=%u\n", m_data_block_number, m_block_size);
  if (m_debug) {std::cout << "end " << __func__ << std::endl; }
}


dataSeparatorRecord::~dataSeparatorRecord() {} 
