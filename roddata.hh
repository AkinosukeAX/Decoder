#ifndef roddata_hh
#define roddata_hh

#include <fstream>
#include <stdint.h>
#include <vector>
#include "algoBase.hh"

class roddata : public algoBase {
public:
  roddata() {}
  roddata(int nwords);
  
  ~roddata();
  
  uint32_t getMarker() const { return m_marker; }
  uint32_t getHeaderSize() const { return m_header_size; }
  uint32_t getAtlasFormatVersion() const { return m_atlas_format_version; }
  uint32_t getRodId() const { return m_rod_id; }
  uint32_t getSourceId() const { return m_source_id; }
  uint32_t getSegmentId() const { return m_segment_id; }
  uint32_t getRunType() const { return m_run_type; }
  uint32_t getRunNumber() const { return m_run_number; }
  uint32_t getLevel1Id() const { return m_level1_id; }
  uint32_t getBcid() const { return m_bcid; }
  uint32_t getTriggerType() const { return m_trigger_type; }
  uint32_t getDetectorEventType() const { return m_detector_event_type; }
  uint32_t getNDataWords() const { return m_ndata_words; }
  const std::vector<uint32_t>& getWords() const { return m_data; }
  void dump() const { for (const auto& word : m_data) {printf("%08x\n", word);} }
  
  
  
private:
  void ProceedFilePointerAndDecodeUser(std::ifstream& inputfile, uint32_t& wordCounter);
  uint32_t m_nwords;
  uint32_t m_marker;
  uint32_t m_header_size;
  uint32_t m_atlas_format_version;
  uint32_t m_rod_id;
  uint32_t m_source_id;
  uint32_t m_segment_id;
  uint32_t m_run_type;
  uint32_t m_run_number;
  uint32_t m_level1_id;
  uint32_t m_bcid;
  uint32_t m_trigger_type;
  uint32_t m_detector_event_type;
  uint32_t m_ndata_words;
  std::vector<uint32_t> m_data;
};


#endif 
