#include "fileNameStrings.hh"
#include <stdio.h>
#include "utils.hh"

fileNameStrings::fileNameStrings() : algoBase() {}

void 
fileNameStrings::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile)
{
  uint32_t bufInt;
  char buf;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  m_marker = bufInt;
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  uint32_t wordLength1 = utils::getWordLength(bufInt);
  uint32_t length1 = bufInt;
  for (uint32_t ii=0; ii<wordLength1; ii++) {
    for (uint32_t iChar=0; iChar<4; iChar++) {
      inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
      if (ii*4+iChar<length1) { m_appName.append(1, buf); }
    }
  }
  //printf("m_appName=%s\n", m_appName.c_str());
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));  
  uint32_t wordLength2 = utils::getWordLength(bufInt);
  uint32_t length2 = bufInt;
  for (uint32_t ii=0; ii<wordLength2; ii++) {
    for (uint32_t iChar=0; iChar<4; iChar++) {
      inputfile.read (reinterpret_cast<char *>(&buf), sizeof (buf));
      if (ii*4+iChar<length2) { m_fileNameCore.append(1, buf); }
    }
  }
  //printf("m_fileNameCore=%s\n", m_fileNameCore.c_str());
}

fileNameStrings::~fileNameStrings() {}
