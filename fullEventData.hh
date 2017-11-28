#ifndef fullEventData_hh
#define fullEventData_hh

#include <fstream>
#include <stdint.h>
#include <vector>

#include "robdata.hh"
#include "algoBase.hh"

class fullEventData : public algoBase {
public:
  fullEventData();
  ~fullEventData();
  const std::vector<robdata>& getRobs() const {return m_robdata;}
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile);

  uint32_t m_marker;
  uint32_t m_total_fragment_size;
  std::vector<robdata> m_robdata;
};

#endif
