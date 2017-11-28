#include "algoBase.hh"

algoBase::algoBase() : m_debug(false), m_algoName("no_name") 
{
}


algoBase::~algoBase()
{
}


void algoBase::ProceedFilePointerAndDecode(std::ifstream& inputfile)
{
  if (m_debug) {std::cout << "start: " << m_algoName << " " <<  __func__ << std::endl; }
  ProceedFilePointerAndDecodeUser(inputfile);
  if (m_debug) {std::cout << "end: " << m_algoName << " " <<  __func__ << std::endl; }
}


void algoBase::ProceedFilePointerAndDecode(std::ifstream& inputfile, uint32_t& wordCounter)
{
  if (m_debug) {std::cout << "start: " << m_algoName << " " <<  __func__ << std::endl; }
  ProceedFilePointerAndDecodeUser(inputfile, wordCounter);
  if (m_debug) {std::cout << "end: " << m_algoName << " " <<  __func__ << std::endl; }
}
