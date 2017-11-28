#ifndef tileCaloROD_hh
#define tileCaloROD_hh

#include "roddata.hh"
#include <vector>

#include "algoBase.hh"

class tileCaloROD : public algoBase {
public:
  tileCaloROD();
  ~tileCaloROD();
  void Analysis(const roddata& rod);
  bool withTMDB() { return (fragment0x40&&fragment0x41&&fragment0x42); }
  uint32_t getAdcNSamples() const { return m_adc_nSamples; }
  const std::vector<std::vector<uint32_t> >& getAdcValues() const { return  m_adc_values; }
  const std::vector<uint32_t>& getEnergyValues() const { return m_energy_values; }
  uint32_t getSlResult0() const { return m_sl_result0; }
  uint32_t getSlResult1() const { return m_sl_result1; }
  uint32_t getSlResult2() const { return m_sl_result2; }
  uint64_t getSlResult() const { return m_sl_result; }
  uint32_t getThrehshold4bits(const int module) const;
  uint32_t getSector() const { return m_sector; }
  bool getIsAside() const { return m_isAside; }
  
private:
  bool fragment0x40;
  bool fragment0x41;
  bool fragment0x42;
  void decode0x40(const std::vector<uint32_t>& words, const uint32_t nwords, uint32_t& wordPointer);
  void decode0x41(const std::vector<uint32_t>& words, const uint32_t nwords, uint32_t& wordPointer);
  void decode0x42(const std::vector<uint32_t>& words, const uint32_t nwords, uint32_t& wordPointer);
  std::vector<std::vector<uint32_t> > m_adc_values;
  std::vector<uint32_t> m_energy_values;
  uint32_t m_adc_nSamples;
  uint32_t m_sl_result0;
  uint32_t m_sl_result1;
  uint32_t m_sl_result2;
  uint64_t m_sl_result;
  uint32_t m_sector;
  bool m_isAside;
};

#endif
