#include "tileCaloROD.hh"

tileCaloROD::tileCaloROD() : algoBase() {}

void 
tileCaloROD::Analysis(const roddata& rod)
{ 
  m_sector  = (rod.getRodId() & 0XF);
  m_isAside = (rod.getSourceId()==0X53);
  
  if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; } 
  fragment0x40=false;
  fragment0x41=false;
  fragment0x42=false;
  
  if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; } 
  const std::vector<uint32_t>& words = rod.getWords();
  const uint32_t nWords = words.size();
  for (uint32_t iWord=0; iWord<nWords; iWord++) {
    const uint32_t& word = words.at(iWord);
    if (word!=0XFF1234FF) { continue; }
    const uint32_t& fragmentSize = words.at(++iWord);
    const uint32_t& fragmentType = words.at(++iWord);
    
    const uint32_t type = ((fragmentType&0XFFFF0000)>>16);
    switch (type) {
    case 0X40:
      fragment0x40=true;
      decode0x40(words, fragmentSize, iWord);
      break;
    case 0X41:
      fragment0x41=true; 
      decode0x41(words, fragmentSize, iWord);
      break;
    case 0X42:
      fragment0x42=true; 
      decode0x42(words, fragmentSize, iWord);
      break;
    default:
      break;
    }
  }
  if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; } 
}

tileCaloROD::~tileCaloROD() {}

void 
tileCaloROD::decode0x40(const std::vector<uint32_t>& words, const uint32_t nwords, uint32_t& wordPointer)
{
  static const uint32_t nChannels = 32;
  
  m_adc_nSamples = (nwords-3)/8;
  m_adc_values.clear();
  for (uint32_t iSample=0; iSample<m_adc_nSamples; iSample++) {
    std::vector<uint32_t> adcs(nChannels);
    for (uint32_t iWord=0; iWord<8; iWord++) {
      const uint32_t& word = words.at(wordPointer);
      for (uint32_t iCh=0; iCh<4; iCh++) {
	const uint32_t channel = iWord*4 + iCh;
	const uint32_t shift = 8*iCh;
	adcs.at(channel) = ((word & (0XFF<<shift))>>shift);
      }
      wordPointer++;
    }
    m_adc_values.push_back(adcs);
  }
}


void 
tileCaloROD::decode0x41(const std::vector<uint32_t>& words, const uint32_t nwords, uint32_t& wordPointer)
{
  m_energy_values.clear();

  if (nwords!=35) {
    printf("decode0x41::ERROR nwords=%u unexpectedly.\n", nwords);
    for (uint32_t iWord=0; iWord<nwords; iWord++) {
      const uint32_t wp=wordPointer+iWord-2;
      printf("tileCaloROD::decode0x41 debug : %08x\n", words.at(wp));
    }
  }
  
  for (uint32_t iWord=0; iWord<32; iWord++) {
    //    const uint32_t& word = words.at(wordPointer); // error modified by Akihiro Higashida, 17/08/24
    const uint32_t& word = words.at(wordPointer + 1);
    const uint32_t channel = iWord;
    m_energy_values.push_back(word);
    wordPointer++;
  }
}


void 
tileCaloROD::decode0x42(const std::vector<uint32_t>& words, const uint32_t nwords, uint32_t& wordPointer)
{
  if (nwords!=5) {
    printf("decode0x42::ERROR nwords=%u unexpectedly.\n", nwords);
  }
  
  const uint64_t word1 = words.at(++wordPointer);
  const uint64_t word2 = words.at(++wordPointer);
  
  m_sl_result0 = (word1 & 0X0000FFFF);
  m_sl_result1 = ((word1 & 0XFFFF0000)>>16);
  m_sl_result2 = (word2 & 0X0000FFFF);
  m_sl_result  = word1 | (word2<<32);
}

uint32_t 
tileCaloROD::getThrehshold4bits(const int module) const
{
  if        (module==0) {
    return ((m_sl_result0 & 0X00F0) >> 4);
  } else if (module==1) {
    return ((m_sl_result0 & 0X0F00) >> 8);
  } else if (module==2) {
    if ((m_sl_result1 & 0X000F)!=((m_sl_result0 & 0XF000)>>12)) {
      std::cout << "decode ERROR @ L:" << __LINE__ << std::endl;
    }
    return ((m_sl_result1 & 0X000F));
  } else if (module==3) {
    return ((m_sl_result1 & 0X00F0) >> 4);
  } else if (module==4) {
    return ((m_sl_result1 & 0X0F00) >> 8);
  } else if (module==5) {
    if ((m_sl_result2 & 0X000F)!=((m_sl_result1 & 0XF000)>>12)) {
      std::cout << "decode ERROR @ L:" << __LINE__ << std::endl;
    }    
    return ((m_sl_result2 & 0X000F));
  } else if (module==6) {
    return ((m_sl_result2 & 0X00F0)>>4);
  } else if (module==7) {
    return ((m_sl_result2 & 0X0F00)>>12);
  } else {
    std::cout << "decode ERROR @ L:" << __LINE__ << " undefined module id=" << module << std::endl;
    return 0XFFFFFFFF;
  }
}

//  LocalWords:  wp
