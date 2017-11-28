#include "robdata.hh"

robdata::robdata() : algoBase() {}

void
robdata::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile, uint32_t& wordCounter)
{
  uint32_t bufInt;
  
  // header 0
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_marker = bufInt;
  
  // header 1
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_nwords = bufInt;
  
  // header 2
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));

  // header 3
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));

  // header 4
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));

  // header 5
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));

  // header 6
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));

  // header 7
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  

  // header 8
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  
  // rod data
  const uint32_t nRodData = m_nwords-9;
  wordCounter=wordCounter+9;
  m_roddata = roddata(nRodData);
  m_roddata.SetDebug(m_debug); m_roddata.SetName("ROD Data");
  m_roddata.ProceedFilePointerAndDecode(inputfile, wordCounter);
}

robdata::~robdata() {}
