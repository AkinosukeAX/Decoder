#ifndef dataSeparatorRecord_hh
#define dataSeparatorRecord_hh

#include <fstream>
#include <stdint.h>
#include "algoBase.hh"

class dataSeparatorRecord : public algoBase 
{
public:
  dataSeparatorRecord();
  ~dataSeparatorRecord();
  uint32_t getBlockSize() {return m_block_size;}
  bool isSeparatorRecord() {return (m_marker==0X1234cccc);}
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile);

  uint32_t m_marker;
  uint32_t m_record_size;
  uint32_t m_data_block_number;
  uint32_t m_block_size;
};

#endif
