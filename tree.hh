#ifndef tree_hh
#define tree_hh

#include <TTree.h>
#include <TFile.h>

#include <string>
#include "fullEventData.hh"
#include "algoBase.hh"

class tree : public algoBase {
public:
  tree() {}
  tree(std::string outputfilename, std::string name, std::string title);
  ~tree();
  void fill(fullEventData& fed);

  void write();
  void setBcidMode(bool bcidmode=true) { m_bcidMode=bcidmode; }
  void enableTgcslBcidCompletion() { m_enable_tgcsl_completion=false; }
  
private:
  void event_clear();
  void fillTgcRod(const roddata& rod);
  void fillTileRod(const roddata& rod);
  void fillTmdbTgcConbinedInfo(); // need to call at the end of the events
  
  uint32_t tmdbBcidDecoder(const uint32_t& word);
  uint32_t tgcslTmdbBcidDecoder(const uint32_t& word);
  uint32_t getTmdbPhiFromTGCSector(const uint32_t tgcSector, 
				   const uint32_t sbloc);
  
  std::vector< uint32_t > m_tmdb_rodid;
  std::vector< uint32_t > m_tmdb_rod_bcid;
  std::vector< uint32_t > m_tmdb_rod_bcid6b; // reduced bcid
  std::vector< uint32_t > m_tmdb_phi; // ROD ID [15:0] - 256
  std::vector< bool > m_tmdb_isAside; // A or C
  std::vector< uint32_t > m_tmdb_sl_result0;
  std::vector< uint32_t > m_tmdb_sl_result1;
  std::vector< uint32_t > m_tmdb_sl_result2;
  std::vector< uint32_t > m_tmdb_sl_result0_bcid;
  std::vector< uint32_t > m_tmdb_sl_result1_bcid;
  std::vector< uint32_t > m_tmdb_sl_result2_bcid;
  std::vector< std::vector<std::vector<uint32_t> > > m_tmdb_adc_values;
  std::vector< std::vector<uint32_t> > m_tmdb_energy_values;
  std::vector< uint32_t > m_tmdb_adc_nsamples;
  
  std::vector< uint32_t > m_tgcsl_rodid_prev;
  std::vector< uint32_t > m_tgcsl_rodid_curr;
  std::vector< uint32_t > m_tgcsl_rodid_next;
  std::vector< uint32_t > m_tgcsl_sector_prev;
  std::vector< uint32_t > m_tgcsl_sector_curr;
  std::vector< uint32_t > m_tgcsl_sector_next;
  std::vector< bool > m_tgcsl_isAside_prev;
  std::vector< bool > m_tgcsl_isAside_curr;
  std::vector< bool > m_tgcsl_isAside_next;
  std::vector< uint32_t > m_tgcsl_rod_bcid_prev;
  std::vector< uint32_t > m_tgcsl_rod_bcid_curr;
  std::vector< uint32_t > m_tgcsl_rod_bcid_next;
  std::vector< uint32_t > m_tgcsl_rod_bcid6b_prev; // reduced bcid
  std::vector< uint32_t > m_tgcsl_rod_bcid6b_curr; // reduced bcid
  std::vector< uint32_t > m_tgcsl_rod_bcid6b_next; // reduced bcid
  std::vector< uint32_t > m_tgcsl_tmdb_tilecalbits_bcid_prev;
  std::vector< uint32_t > m_tgcsl_tmdb_tilecalbits_bcid_curr;
  std::vector< uint32_t > m_tgcsl_tmdb_tilecalbits_bcid_next;
  std::vector< uint32_t > m_tgcsl_tmdb_loc_prev;
  std::vector< uint32_t > m_tgcsl_tmdb_loc_curr;
  std::vector< uint32_t > m_tgcsl_tmdb_loc_next;
  std::vector< uint32_t > m_tgcsl_tmdb_tilecalbits_prev;
  std::vector< uint32_t > m_tgcsl_tmdb_tilecalbits_curr;
  std::vector< uint32_t > m_tgcsl_tmdb_tilecalbits_next;



  // for debugging purpose
  std::vector< uint32_t > m_tgcsl_rodid;
  std::vector< uint32_t > m_tgcsl_sectorid;
  std::vector< bool > m_tgcsl_isAside;
  std::vector< uint32_t > m_tgcsl_rod_bcid;
  std::vector< uint32_t > m_tgcsl_rod_bcid6b;
  std::vector< unsigned long long > m_tgcsl_correspondingtile0;
  std::vector< unsigned long long > m_tgcsl_correspondingtile1;
  std::vector< unsigned long long > m_tgcsl_correspondingtile2;
  std::vector< unsigned long long > m_tgcsl_correspondingtile3;
  std::vector< uint32_t > m_tgcsl_correspondingtile0_bcid;
  std::vector< uint32_t > m_tgcsl_correspondingtile1_bcid;
  std::vector< uint32_t > m_tgcsl_correspondingtile2_bcid;
  std::vector< uint32_t > m_tgcsl_correspondingtile3_bcid;
  std::vector< uint32_t > m_tgcsl_bctiming;
  std::vector< uint32_t > m_tgcsl_tilecalobits0;
  std::vector< uint32_t > m_tgcsl_tilecalobits1;
  std::vector< uint32_t > m_tgcsl_tilecalobits2;
  std::vector< uint32_t > m_tgcsl_tilecalobits3;
  std::vector< uint32_t > m_tgcsl_tilecalobits0_bcid;
  std::vector< uint32_t > m_tgcsl_tilecalobits1_bcid;
  std::vector< uint32_t > m_tgcsl_tilecalobits2_bcid;
  std::vector< uint32_t > m_tgcsl_tilecalobits3_bcid;
  std::vector< uint32_t > m_tgcsl_coin_bit0;
  std::vector< uint32_t > m_tgcsl_coin_bit1;
  std::vector< uint32_t > m_tgcsl_coin_bit2;
  std::vector< uint32_t > m_tgcsl_coin_bit3;
  
  TTree* m_tree;
  TFile* m_file;
  
  uint32_t nTgcRods;
  uint32_t nTmdbRods;
  
  bool m_bcidMode;
  bool m_enable_tgcsl_completion; // for zero suppression at ROD
};

#endif
