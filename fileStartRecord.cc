#include "fileStartRecord.hh"

fileStartRecord::fileStartRecord() : algoBase() {}

void 
fileStartRecord::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile)
{
  uint32_t bufInt;
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_marker = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_record_size = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_version = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_file_number = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_date = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_time = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_sizeLimit_dataBlocks = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_sizeLimit_MB = bufInt;
}


fileStartRecord::~fileStartRecord() {}

