#ifndef __ALGOBASE__
#define __ALGOBASE__

#include <iostream>
#include <string>

class algoBase {
  
public:
  algoBase();
  ~algoBase();
  void SetDebug(bool debug) { m_debug = debug; }
  void SetName(std::string name) { m_algoName = name; }
  void ProceedFilePointerAndDecode(std::ifstream& inputfile);
  void ProceedFilePointerAndDecode(std::ifstream& inputfile, uint32_t& wordCounter);

  
protected:
  bool m_debug;
  std::string m_algoName;
  virtual void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile){};
  virtual void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile, uint32_t& wordCounter){};
};

#endif

