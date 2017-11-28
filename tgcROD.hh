#ifndef tgcROD_hh
#define tgcROD_hh

#include "roddata.hh"
#include "algoBase.hh"

class tgcROD : public algoBase {
public:
  tgcROD();
  ~tgcROD();
  
  void Analysis(const roddata& rod);

  const std::vector<uint32_t>& getSlTmdbTilecalbitsPrev() const { return m_sl_tmdb_tilecalbits_prev; }
  const std::vector<uint32_t>& getSlTmdbLocPrev() const { return m_sl_tmdb_loc_prev; }
  const std::vector<uint32_t>& getSlTmdbTilecalbitsCurr() const { return m_sl_tmdb_tilecalbits_curr; }
  const std::vector<uint32_t>& getSlTmdbLocCurr() const { return m_sl_tmdb_loc_curr; }
  const std::vector<uint32_t>& getSlTmdbTilecalbitsNext() const { return m_sl_tmdb_tilecalbits_next; }
  const std::vector<uint32_t>& getSlTmdbLocNext() const { return m_sl_tmdb_loc_next; }
  const std::string& getSide() const { return m_side; }

  
  // Added by Akihiro Higashida 171002
  const std::vector<bool>& getSlCoinVetoPrev() const { return m_sl_coin_veto_prev; }
  const std::vector<uint32_t>& getSlCoinbitPrev() const { return m_sl_coin_bit_prev; }
  const std::vector<uint32_t>& getSlCoinLocPrev() const { return m_sl_coin_loc_prev; }  
  const std::vector<bool>& getSlCoinVetoCurr() const { return m_sl_coin_veto_curr; }
  const std::vector<uint32_t>& getSlCoinbitCurr() const { return m_sl_coin_bit_curr; }
  const std::vector<uint32_t>& getSlCoinLocCurr() const { return m_sl_coin_loc_curr; }  
  const std::vector<bool>& getSlCoinVetoNext() const { return m_sl_coin_veto_next; }
  const std::vector<uint32_t>& getSlCoinbitNext() const { return m_sl_coin_bit_next; }
  const std::vector<uint32_t>& getSlCoinLocNext() const { return m_sl_coin_loc_next; }  
  
  int getSector() const { return m_sector; }
    // Added by Akihiro Higashida 171002

private:
  std::vector<uint32_t> m_sl_tmdb_tilecalbits_prev;
  std::vector<uint32_t> m_sl_tmdb_loc_prev;
  std::vector<uint32_t> m_sl_tmdb_tilecalbits_curr;
  std::vector<uint32_t> m_sl_tmdb_loc_curr;
  std::vector<uint32_t> m_sl_tmdb_tilecalbits_next;
  std::vector<uint32_t> m_sl_tmdb_loc_next;

  
  std::vector<bool> m_sl_coin_veto_prev;
  std::vector<uint32_t> m_sl_coin_bit_prev;
  std::vector<uint32_t> m_sl_coin_loc_prev;
  std::vector<bool> m_sl_coin_veto_curr;
  std::vector<uint32_t> m_sl_coin_bit_curr;
  std::vector<uint32_t> m_sl_coin_loc_curr;
  std::vector<bool> m_sl_coin_veto_next;
  std::vector<uint32_t> m_sl_coin_bit_next;
  std::vector<uint32_t> m_sl_coin_loc_next;
  
  std::string m_side;
  int m_sector;
  void highPtReadoutFormat(const uint32_t& word);
  void SLReadoutFormat(const uint32_t& word);
  void clear();
};

#endif 
