#ifndef robdata_hh
#define robdata_hh

#include <fstream>
#include <stdint.h>
#include "roddata.hh" 

#include "algoBase.hh"

class robdata : public algoBase {
public:
  robdata();
  ~robdata();
  const roddata& getRod() const {return m_roddata;}
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile, uint32_t& wordCounter);
  
  uint32_t m_marker;
  uint32_t m_rob_id;
  uint32_t m_nwords;
  roddata m_roddata;
};

#endif
