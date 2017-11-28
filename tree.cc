#include "tree.hh"
#include "tileCaloROD.hh"
#include "tgcROD.hh"

#include <map>

#include <inttypes.h>

uint32_t
tree::getTmdbPhiFromTGCSector(const uint32_t tgcSector, 
			      const uint32_t sbloc)
{
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  const uint32_t tgcTriggerSector = (tgcSector-1)*4+sbloc;
  
  if        (tgcTriggerSector>=2 && tgcTriggerSector<=7)  {
    return 0X0;
  } else if (tgcTriggerSector>=8 && tgcTriggerSector<=13)  {
    return 0X1;
  } else if (tgcTriggerSector>=14 && tgcTriggerSector<=19)  {
    return 0X2;
  } else if (tgcTriggerSector>=20 && tgcTriggerSector<=25) { 
    return 0X3;
  } else if (tgcTriggerSector>=26 && tgcTriggerSector<=31)  {
    return 0X4;
  } else if (tgcTriggerSector>=32 && tgcTriggerSector<=37)  {
    return 0X5;
  } else if (tgcTriggerSector>=38 && tgcTriggerSector<=43)  {
    return 0X6;
  } else if (tgcTriggerSector>=44 && tgcTriggerSector<=47)  {
    return 0X7;
  } else if (tgcTriggerSector>=0 && tgcTriggerSector<=1)  {
    return 0X7;
  } else {
    printf ("\033[91m tree::getTmdbPhiFromTGCSector ERROR unexpected tgcTriggerSector=%d \033[0m\n", 
	    tgcTriggerSector);
    return 0XFFFFFFFF;
  }
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
}

tree::tree(std::string outputfilename, std::string name, std::string title) : algoBase() 
{
  m_file = new TFile(outputfilename.c_str(), "RECREATE");
  m_tree = new TTree(name.c_str(), title.c_str());
  m_tree->Branch("nTgcRods", &nTgcRods);
  m_tree->Branch("nTmdbRods", &nTmdbRods);
  m_tree->Branch("tmdb_rodid", &m_tmdb_rodid);
  m_tree->Branch("tmdb_isAside", &m_tmdb_isAside);
  m_tree->Branch("tmdb_rod_bcid", &m_tmdb_rod_bcid);
  m_tree->Branch("tmdb_rod_bcid6b", &m_tmdb_rod_bcid6b);
  m_tree->Branch("tmdb_phi", &m_tmdb_phi); //  rod id & 0X00FFFF - 256
  m_tree->Branch("tmdb_adc_values", &m_tmdb_adc_values);
  m_tree->Branch("tmdb_adc_nsamples", &m_tmdb_adc_nsamples);
  m_tree->Branch("tmdb_energy_values", &m_tmdb_energy_values);
  m_tree->Branch("tmdb_sl_result0", &m_tmdb_sl_result0);
  m_tree->Branch("tmdb_sl_result1", &m_tmdb_sl_result1);
  m_tree->Branch("tmdb_sl_result2", &m_tmdb_sl_result2);
  m_tree->Branch("tmdb_sl_result0_bcid", &m_tmdb_sl_result0_bcid);
  m_tree->Branch("tmdb_sl_result1_bcid", &m_tmdb_sl_result1_bcid);
  m_tree->Branch("tmdb_sl_result2_bcid", &m_tmdb_sl_result2_bcid);
  m_tree->Branch("tgcsl_rodid_prev", &m_tgcsl_rodid_prev);
  m_tree->Branch("tgcsl_rodid_curr", &m_tgcsl_rodid_curr);
  m_tree->Branch("tgcsl_rodid_next", &m_tgcsl_rodid_next);
  m_tree->Branch("tgcsl_rod_bcid_prev", &m_tgcsl_rod_bcid_prev);
  m_tree->Branch("tgcsl_rod_bcid_curr", &m_tgcsl_rod_bcid_curr);
  m_tree->Branch("tgcsl_rod_bcid_next", &m_tgcsl_rod_bcid_next);
  m_tree->Branch("tgcsl_rod_bcid6b_prev", &m_tgcsl_rod_bcid6b_prev);
  m_tree->Branch("tgcsl_rod_bcid6b_curr", &m_tgcsl_rod_bcid6b_curr);
  m_tree->Branch("tgcsl_rod_bcid6b_next", &m_tgcsl_rod_bcid6b_next);
  m_tree->Branch("tgcsl_sector_prev", &m_tgcsl_sector_prev);
  m_tree->Branch("tgcsl_sector_curr", &m_tgcsl_sector_curr);
  m_tree->Branch("tgcsl_sector_next", &m_tgcsl_sector_next);
  m_tree->Branch("tgcsl_isAside_prev", &m_tgcsl_isAside_prev);
  m_tree->Branch("tgcsl_isAside_curr", &m_tgcsl_isAside_curr);
  m_tree->Branch("tgcsl_isAside_next", &m_tgcsl_isAside_next);
  m_tree->Branch("tgcsl_tmdb_tilecalbits_prev", &m_tgcsl_tmdb_tilecalbits_prev);
  m_tree->Branch("tgcsl_tmdb_loc_prev", &m_tgcsl_tmdb_loc_prev);
  m_tree->Branch("tgcsl_tmdb_tilecalbits_curr", &m_tgcsl_tmdb_tilecalbits_curr);
  m_tree->Branch("tgcsl_tmdb_loc_curr", &m_tgcsl_tmdb_loc_curr);
  m_tree->Branch("tgcsl_tmdb_tilecalbits_next", &m_tgcsl_tmdb_tilecalbits_next);
  m_tree->Branch("tgcsl_tmdb_loc_next", &m_tgcsl_tmdb_loc_next);
  m_tree->Branch("tgcsl_tmdb_tilecalbits_bcid_prev", &m_tgcsl_tmdb_tilecalbits_bcid_prev);
  m_tree->Branch("tgcsl_tmdb_tilecalbits_bcid_curr", &m_tgcsl_tmdb_tilecalbits_bcid_curr);
  m_tree->Branch("tgcsl_tmdb_tilecalbits_bcid_next", &m_tgcsl_tmdb_tilecalbits_bcid_next);
  // for debugging purpose
  m_tree->Branch("tgcsl_rodid", &m_tgcsl_rodid);
  m_tree->Branch("tgcsl_sectorid", &m_tgcsl_sectorid);
  m_tree->Branch("tgcsl_isAside", &m_tgcsl_isAside);
  m_tree->Branch("tgcsl_rod_bcid", &m_tgcsl_rod_bcid);
  m_tree->Branch("tgcsl_rod_bcid6b", &m_tgcsl_rod_bcid6b);
  m_tree->Branch("tgcsl_correspondingtile0_bcid", &m_tgcsl_correspondingtile0_bcid);
  m_tree->Branch("tgcsl_correspondingtile1_bcid", &m_tgcsl_correspondingtile1_bcid);
  m_tree->Branch("tgcsl_correspondingtile2_bcid", &m_tgcsl_correspondingtile2_bcid);
  m_tree->Branch("tgcsl_correspondingtile3_bcid", &m_tgcsl_correspondingtile3_bcid);
  m_tree->Branch("tgcsl_correspondingtile0", &m_tgcsl_correspondingtile0); // unsigned long long
  m_tree->Branch("tgcsl_correspondingtile1", &m_tgcsl_correspondingtile1); // unsigned long long
  m_tree->Branch("tgcsl_correspondingtile2", &m_tgcsl_correspondingtile2); // unsigned long long
  m_tree->Branch("tgcsl_correspondingtile3", &m_tgcsl_correspondingtile3); // unsigned long long
  m_tree->Branch("tgcsl_bctiming", &m_tgcsl_bctiming);
  m_tree->Branch("tgcsl_tilecalobits0", &m_tgcsl_tilecalobits0);
  m_tree->Branch("tgcsl_tilecalobits1", &m_tgcsl_tilecalobits1);
  m_tree->Branch("tgcsl_tilecalobits2", &m_tgcsl_tilecalobits2);
  m_tree->Branch("tgcsl_tilecalobits3", &m_tgcsl_tilecalobits3);
  m_tree->Branch("tgcsl_tilecalobits0_bcid", &m_tgcsl_tilecalobits0_bcid);
  m_tree->Branch("tgcsl_tilecalobits1_bcid", &m_tgcsl_tilecalobits1_bcid);
  m_tree->Branch("tgcsl_tilecalobits2_bcid", &m_tgcsl_tilecalobits2_bcid);
  m_tree->Branch("tgcsl_tilecalobits3_bcid", &m_tgcsl_tilecalobits3_bcid);

  m_tree->Branch("tgcsl_coin_bit0", &m_tgcsl_coin_bit0);
  m_tree->Branch("tgcsl_coin_bit1", &m_tgcsl_coin_bit1);
  m_tree->Branch("tgcsl_coin_bit2", &m_tgcsl_coin_bit2);
  m_tree->Branch("tgcsl_coin_bit3", &m_tgcsl_coin_bit3);
  
  m_bcidMode = false;
  m_enable_tgcsl_completion = false;
}

tree::~tree() {}

void tree::fillTgcRod(const roddata& rod)
{
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  nTgcRods++;
  tgcROD tgcRod; 
  tgcRod.SetDebug(m_debug); tgcRod.SetName("TGC ROD"); 
  tgcRod.Analysis(rod);
  //rod.dump();
  
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }

  std::vector<uint32_t> tilecalbits_prev = tgcRod.getSlTmdbTilecalbitsPrev();
  std::vector<uint32_t> loc_prev = tgcRod.getSlTmdbLocPrev();
  std::vector<uint32_t> tilecalbits_curr = tgcRod.getSlTmdbTilecalbitsCurr();
  std::vector<uint32_t> loc_curr = tgcRod.getSlTmdbLocCurr();
  std::vector<uint32_t> tilecalbits_next = tgcRod.getSlTmdbTilecalbitsNext();
  std::vector<uint32_t> loc_next = tgcRod.getSlTmdbLocNext();
  
  //  std::vector<bool> sl_coin_veto_prev = tgcRod.getSlCoinVetoPrev();
  std::vector<uint32_t> sl_coin_loc_prev = tgcRod.getSlCoinLocPrev();
  std::vector<uint32_t> sl_coin_bit_prev = tgcRod.getSlCoinbitPrev();
  // //  std::vector<bool> sl_coin_veto_curr = tgcRod.getSlCoinVetoCurr();
  std::vector<uint32_t> sl_coin_loc_curr = tgcRod.getSlCoinLocCurr();
  std::vector<uint32_t> sl_coin_bit_curr = tgcRod.getSlCoinbitCurr();
  // //  std::vector<bool> sl_coin_veto_next = tgcRod.getSlCoinVetoNext();
  std::vector<uint32_t> sl_coin_loc_next = tgcRod.getSlCoinLocNext();
   std::vector<uint32_t> sl_coin_bit_next = tgcRod.getSlCoinbitNext();
  
  if (m_bcidMode) { // for completion 'for zero suppression at ROD'
    std::map<uint32_t, uint32_t> sl_locBCMap;
    const uint32_t sl_bcmapPrev=0X4, sl_bcmapCurr=0X2, sl_bcmapNext=0X1;
    for (const auto& loc : sl_coin_loc_prev) { // previous
      if (sl_locBCMap.find(loc) == sl_locBCMap.end()) {sl_locBCMap.insert(std::make_pair(loc, sl_bcmapPrev));}
      else { sl_locBCMap.at(loc) = (sl_locBCMap.at(loc)|sl_bcmapPrev); }
    }
    for (const auto& loc : sl_coin_loc_curr) { // current
      if (sl_locBCMap.find(loc) == sl_locBCMap.end()) {sl_locBCMap.insert(std::make_pair(loc, sl_bcmapCurr));}
      else { sl_locBCMap.at(loc) = (sl_locBCMap.at(loc)|sl_bcmapCurr); }
    }
    for (const auto& loc : sl_coin_loc_next) { // next
      if (sl_locBCMap.find(loc) == sl_locBCMap.end()) {sl_locBCMap.insert(std::make_pair(loc, sl_bcmapNext));}
      else { sl_locBCMap.at(loc) = (sl_locBCMap.at(loc)|sl_bcmapNext); }
    }
    
    for (const auto& sl_locBCPair : sl_locBCMap) {
      if (sl_locBCPair.second == 0X7) { continue; } // already completed
      switch (sl_locBCPair.second) {
      case 0X3: // previous is missing because of 'zero suppression in ROD'
	if (m_enable_tgcsl_completion) {
	  sl_coin_loc_prev.push_back(sl_locBCPair.first);
	  sl_coin_bit_prev.push_back(0X0);
	} else {
	  //	  printf("\033[94m tree::fillTgcRod WARNING possibly recoverable missing bcmap=%x loc=%d (likely due to zero suppression at ROD) \033[0m\n ", sl_locBCPair.second, sl_locBCPair.first);
	}
	break;
      case 0X5: // current is missing because of 'zero suppression in ROD'
	if (m_enable_tgcsl_completion) {
	  sl_coin_loc_curr.push_back(sl_locBCPair.first);
	  sl_coin_bit_curr.push_back(0X0);
	} else {
	  //	  printf("\033[94m tree::fillTgcRod WARNING possibly recoverable missing bcmap=%x loc=%d (likely due to zero suppression at ROD) \033[0m\n ", sl_locBCPair.second, sl_locBCPair.first);
	}
	break;
      case 0X6: // next is missing because of 'zero suppression in ROD'
	if (m_enable_tgcsl_completion) {
	  sl_coin_loc_next.push_back(sl_locBCPair.first);
	  sl_coin_bit_next.push_back(0X0);
	} else {
	  //	  printf("\033[94m tree::fillTgcRod WARNING possibly recoverable missing bcmap=%x loc=%d (likely due to zero suppression at ROD) \033[0m\n ", sl_locBCPair.second, sl_locBCPair.first);
	}
	break;
      default:
	// printf("\033[91m tree::fillTgcRod ERROR unexpected missing bcmap=%x loc=%d \033[0m\n ", sl_locBCPair.second, sl_locBCPair.first);
	// printf("rodid=%08x l1id=%08x \n", rod.getRodId(), rod.getLevel1Id());
	//rod.dump();
	break;
      }
    }
  
  
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
    // if (m_bcidMode) { // for completion 'for zero suppression at ROD'

    std::map<uint32_t, uint32_t> locBCMap;
    const uint32_t bcmapPrev=0X4, bcmapCurr=0X2, bcmapNext=0X1;
    for (const auto& loc : loc_prev) { // previous
      if (locBCMap.find(loc) == locBCMap.end()) {locBCMap.insert(std::make_pair(loc, bcmapPrev));}
      else { locBCMap.at(loc) = (locBCMap.at(loc)|bcmapPrev); }
    }
    for (const auto& loc : loc_curr) { // current
      if (locBCMap.find(loc) == locBCMap.end()) {locBCMap.insert(std::make_pair(loc, bcmapCurr));}
      else { locBCMap.at(loc) = (locBCMap.at(loc)|bcmapCurr); }
    }
    for (const auto& loc : loc_next) { // next
      if (locBCMap.find(loc) == locBCMap.end()) {locBCMap.insert(std::make_pair(loc, bcmapNext));}
      else { locBCMap.at(loc) = (locBCMap.at(loc)|bcmapNext); }
    }
    
    for (const auto& locBCPair : locBCMap) {
      if (locBCPair.second == 0X7) { continue; } // already completed
      switch (locBCPair.second) {
      case 0X3: // previous is missing because of 'zero suppression in ROD'
	if (m_enable_tgcsl_completion) {
	  loc_prev.push_back(locBCPair.first);
	  tilecalbits_prev.push_back(0X0);
	} else {
	  //	  printf("\033[94m tree::fillTgcRod WARNING possibly recoverable missing bcmap=%x loc=%d (likely due to zero suppression at ROD) \033[0m\n ", locBCPair.second, locBCPair.first);
	}
	break;
      case 0X5: // current is missing because of 'zero suppression in ROD'
	if (m_enable_tgcsl_completion) {
	  loc_curr.push_back(locBCPair.first);
	  tilecalbits_curr.push_back(0X0);
	} else {
	  //  printf("\033[94m tree::fillTgcRod WARNING possibly recoverable missing bcmap=%x loc=%d (likely due to zero suppression at ROD) \033[0m\n ", locBCPair.second, locBCPair.first);
	}
	break;
      case 0X6: // next is missing because of 'zero suppression in ROD'
	if (m_enable_tgcsl_completion) {
	  loc_next.push_back(locBCPair.first);
	  tilecalbits_next.push_back(0X0);
	} else {
	  //	  printf("\033[94m tree::fillTgcRod WARNING possibly recoverable missing bcmap=%x loc=%d (likely due to zero suppression at ROD) \033[0m\n ", locBCPair.second, locBCPair.first);
	}
	break;
      default:
	// printf("\033[91m tree::fillTgcRod ERROR unexpected missing bcmap=%x loc=%d \033[0m\n ", locBCPair.second, locBCPair.first);
	// printf("rodid=%08x l1id=%08x \n", rod.getRodId(), rod.getLevel1Id());
	//rod.dump();
	break;
      }
    }
    
    // for debugging purpose
    //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
    std::vector<uint32_t> mapPrev = {0XFFFFFF, 0XFFFFFF, 0XFFFFFF, 0XFFFFFF};
    std::vector<uint32_t> mapCurr = {0XFFFFFF, 0XFFFFFF, 0XFFFFFF, 0XFFFFFF};
    std::vector<uint32_t> mapNext = {0XFFFFFF, 0XFFFFFF, 0XFFFFFF, 0XFFFFFF};
    for (uint32_t iLoc=0, nLocs=loc_prev.size(); iLoc<nLocs; iLoc++) {mapPrev[loc_prev.at(iLoc)]=tilecalbits_prev.at(iLoc); }
    for (uint32_t iLoc=0, nLocs=loc_curr.size(); iLoc<nLocs; iLoc++) {mapCurr[loc_curr.at(iLoc)]=tilecalbits_curr.at(iLoc); }
    for (uint32_t iLoc=0, nLocs=loc_next.size(); iLoc<nLocs; iLoc++) {mapNext[loc_next.at(iLoc)]=tilecalbits_next.at(iLoc); }

    std::vector<uint32_t> mapslPrev = {0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF};
    std::vector<uint32_t> mapslCurr = {0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF};
    std::vector<uint32_t> mapslNext = {0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF};
    
    for (uint32_t iLoc=0, nLocs=sl_coin_loc_prev.size(); iLoc<nLocs; iLoc++) {mapslPrev[sl_coin_loc_prev.at(iLoc)]=sl_coin_bit_prev.at(iLoc);}
    for (uint32_t iLoc=0, nLocs=sl_coin_loc_curr.size(); iLoc<nLocs; iLoc++) {mapslCurr[sl_coin_loc_curr.at(iLoc)]=sl_coin_bit_curr.at(iLoc);}
    for (uint32_t iLoc=0, nLocs=sl_coin_loc_next.size(); iLoc<nLocs; iLoc++) {mapslNext[sl_coin_loc_next.at(iLoc)]=sl_coin_bit_next.at(iLoc);}
     
    m_tgcsl_rodid.push_back(rod.getRodId());
    m_tgcsl_sectorid.push_back((rod.getRodId()&0XF));
    m_tgcsl_isAside.push_back( ((rod.getRodId()&0X00FF0000)==0X00670000) );
    
    m_tgcsl_rod_bcid.push_back(rod.getBcid());
    m_tgcsl_rod_bcid6b.push_back( (rod.getBcid()&0X3F) );
    m_tgcsl_bctiming.push_back(0); // previous
    m_tgcsl_tilecalobits0.push_back(mapPrev.at(0)); m_tgcsl_tilecalobits1.push_back(mapPrev.at(1)); m_tgcsl_tilecalobits2.push_back(mapPrev.at(2)); m_tgcsl_tilecalobits3.push_back(mapPrev.at(3));
    m_tgcsl_tilecalobits0_bcid.push_back(tgcslTmdbBcidDecoder(mapPrev.at(0))); 
    m_tgcsl_tilecalobits1_bcid.push_back(tgcslTmdbBcidDecoder(mapPrev.at(1))); 
    m_tgcsl_tilecalobits2_bcid.push_back(tgcslTmdbBcidDecoder(mapPrev.at(2))); 
    m_tgcsl_tilecalobits3_bcid.push_back(tgcslTmdbBcidDecoder(mapPrev.at(3))); 
    m_tgcsl_coin_bit0.push_back(mapslPrev.at(0));
    m_tgcsl_coin_bit1.push_back(mapslPrev.at(1));
    m_tgcsl_coin_bit2.push_back(mapslPrev.at(2));
    m_tgcsl_coin_bit3.push_back(mapslPrev.at(3));
    
    m_tgcsl_rodid.push_back(rod.getRodId());
    m_tgcsl_sectorid.push_back((rod.getRodId()&0XF));
    m_tgcsl_isAside.push_back( ((rod.getRodId()&0X00FF0000)==0X00670000) );
    m_tgcsl_rod_bcid.push_back(rod.getBcid());
    m_tgcsl_rod_bcid6b.push_back( (rod.getBcid()&0X3F) );
    m_tgcsl_bctiming.push_back(1); // current
    m_tgcsl_tilecalobits0.push_back(mapCurr.at(0)); m_tgcsl_tilecalobits1.push_back(mapCurr.at(1)); m_tgcsl_tilecalobits2.push_back(mapCurr.at(2)); m_tgcsl_tilecalobits3.push_back(mapCurr.at(3));
    m_tgcsl_tilecalobits0_bcid.push_back(tgcslTmdbBcidDecoder(mapCurr.at(0))); 
    m_tgcsl_tilecalobits1_bcid.push_back(tgcslTmdbBcidDecoder(mapCurr.at(1))); 
    m_tgcsl_tilecalobits2_bcid.push_back(tgcslTmdbBcidDecoder(mapCurr.at(2))); 
    m_tgcsl_tilecalobits3_bcid.push_back(tgcslTmdbBcidDecoder(mapCurr.at(3))); 
    m_tgcsl_coin_bit0.push_back(mapslCurr.at(0));
    m_tgcsl_coin_bit1.push_back(mapslCurr.at(1));
    m_tgcsl_coin_bit2.push_back(mapslCurr.at(2));
    m_tgcsl_coin_bit3.push_back(mapslCurr.at(3));
     
    
    m_tgcsl_rodid.push_back(rod.getRodId());
    m_tgcsl_sectorid.push_back((rod.getRodId()&0XF));
    m_tgcsl_isAside.push_back( ((rod.getRodId()&0X00FF0000)==0X00670000) );
    m_tgcsl_rod_bcid.push_back(rod.getBcid());
    m_tgcsl_rod_bcid6b.push_back( (rod.getBcid()&0X3F) );
    m_tgcsl_bctiming.push_back(2); // next
    m_tgcsl_tilecalobits0.push_back(mapNext.at(0)); m_tgcsl_tilecalobits1.push_back(mapNext.at(1)); m_tgcsl_tilecalobits2.push_back(mapNext.at(2)); m_tgcsl_tilecalobits3.push_back(mapNext.at(3));
    m_tgcsl_tilecalobits0_bcid.push_back(tgcslTmdbBcidDecoder(mapNext.at(0))); 
    m_tgcsl_tilecalobits1_bcid.push_back(tgcslTmdbBcidDecoder(mapNext.at(1))); 
    m_tgcsl_tilecalobits2_bcid.push_back(tgcslTmdbBcidDecoder(mapNext.at(2))); 
    m_tgcsl_tilecalobits3_bcid.push_back(tgcslTmdbBcidDecoder(mapNext.at(3))); 
    m_tgcsl_coin_bit0.push_back(mapslNext.at(0));
    m_tgcsl_coin_bit1.push_back(mapslNext.at(1));
    m_tgcsl_coin_bit2.push_back(mapslNext.at(2));
    m_tgcsl_coin_bit3.push_back(mapslNext.at(3));

    //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  }
  
  
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  for (uint32_t ii=0, n=tilecalbits_prev.size(); ii<n; ii++) {
    m_tgcsl_rodid_prev.push_back(rod.getRodId());
    m_tgcsl_rod_bcid_prev.push_back(rod.getBcid());
    m_tgcsl_rod_bcid6b_prev.push_back((rod.getBcid()&0X3F));
    m_tgcsl_sector_prev.push_back(rod.getRodId()&0XF);
    m_tgcsl_isAside_prev.push_back( ((rod.getRodId()&0XFF0000)==0X670000) );
    m_tgcsl_tmdb_tilecalbits_prev.push_back(tilecalbits_prev.at(ii));
    m_tgcsl_tmdb_loc_prev.push_back(loc_prev.at(ii));
    if (m_bcidMode) { m_tgcsl_tmdb_tilecalbits_bcid_prev.push_back(tgcslTmdbBcidDecoder(tilecalbits_prev.at(ii))); }
  }
  
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  for (uint32_t ii=0, n=tilecalbits_curr.size(); ii<n; ii++) {
    m_tgcsl_rodid_curr.push_back(rod.getRodId());
    m_tgcsl_rod_bcid_curr.push_back(rod.getBcid());
    m_tgcsl_rod_bcid6b_curr.push_back((rod.getBcid()&0X3F));
    m_tgcsl_sector_curr.push_back(rod.getRodId()&0XF);
    m_tgcsl_isAside_curr.push_back( ((rod.getRodId()&0XFF0000)==0X670000) );
    m_tgcsl_tmdb_tilecalbits_curr.push_back(tilecalbits_curr.at(ii));
    m_tgcsl_tmdb_loc_curr.push_back(loc_curr.at(ii));
    if (m_bcidMode) { m_tgcsl_tmdb_tilecalbits_bcid_curr.push_back(tgcslTmdbBcidDecoder(tilecalbits_curr.at(ii))); }
  }
  
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  for (uint32_t ii=0, n=tilecalbits_next.size(); ii<n; ii++) {
    m_tgcsl_rodid_next.push_back(rod.getRodId());
    m_tgcsl_rod_bcid_next.push_back(rod.getBcid());
    m_tgcsl_rod_bcid6b_next.push_back((rod.getBcid()&0X3F));
    m_tgcsl_sector_next.push_back(rod.getRodId()&0XF);
    m_tgcsl_isAside_next.push_back( ((rod.getRodId()&0XFF0000)==0X670000) );
    m_tgcsl_tmdb_tilecalbits_next.push_back(tilecalbits_next.at(ii));
    m_tgcsl_tmdb_loc_next.push_back(loc_next.at(ii));
    if (m_bcidMode) { m_tgcsl_tmdb_tilecalbits_bcid_next.push_back(tgcslTmdbBcidDecoder(tilecalbits_next.at(ii))); }
  }
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
}


uint32_t tree::tmdbBcidDecoder(const uint32_t& word)
{
  uint32_t bit1 = ((word&0X00000010) >> 4);
  uint32_t bit0 = ((word&0X00000040) >> 6);
  uint32_t bit3 = ((word&0X00000100) >> 8);
  uint32_t bit2 = ((word&0X00000400) >> 10);
  uint32_t bit5 = ((word&0X00001000) >> 12);
  uint32_t bit4 = ((word&0X00004000) >> 14);
  return (bit0 + (bit1<<1) + (bit2<<2) + (bit3<<3) + (bit4<<4) + (bit5<<5) );
}


uint32_t tree::tgcslTmdbBcidDecoder(const uint32_t& word)
{
  return (word>>2);
}


void tree::fillTileRod(const roddata& rod)
{
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  tileCaloROD tileRod;  
  tileRod.SetDebug(m_debug); tileRod.SetName("Tile ROD"); 
  tileRod.Analysis(rod);
  
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  if (tileRod.withTMDB()) {
    //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
    //rod.dump();
    nTmdbRods++;
    m_tmdb_rodid.push_back(rod.getRodId());
    m_tmdb_isAside.push_back( ((rod.getRodId()&0XFF0000)==0X530000) );
    m_tmdb_rod_bcid.push_back(rod.getBcid());
    m_tmdb_rod_bcid6b.push_back( (rod.getBcid()&0X3F) );
    m_tmdb_phi.push_back((rod.getRodId() & 0XFFFF)-256);
    m_tmdb_sl_result0.push_back(tileRod.getSlResult0());
    m_tmdb_sl_result1.push_back(tileRod.getSlResult1());
    m_tmdb_sl_result2.push_back(tileRod.getSlResult2());
    m_tmdb_adc_values.push_back(tileRod.getAdcValues());
    m_tmdb_adc_nsamples.push_back(tileRod.getAdcNSamples());
    m_tmdb_energy_values.push_back(tileRod.getEnergyValues());
    //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
    if (m_bcidMode) {
      const uint32_t bcid0 = tmdbBcidDecoder(tileRod.getSlResult0());
      const uint32_t bcid1 = tmdbBcidDecoder(tileRod.getSlResult1());
      const uint32_t bcid2 = tmdbBcidDecoder(tileRod.getSlResult2());
      m_tmdb_sl_result0_bcid.push_back(bcid0);
      m_tmdb_sl_result1_bcid.push_back(bcid1);
      m_tmdb_sl_result2_bcid.push_back(bcid2);
      if ( !((bcid0==bcid1) and (bcid1==bcid2)) ) {
	// printf ("\033[91m void tree::fillTileRod inconsistent bcid %d %d %d \033[0m\n", 
	// 	bcid0, bcid1, bcid2);
      }
    }
    //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  }
}


void 
tree::fill(fullEventData& fed)
{
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  event_clear();
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  const std::vector<robdata>& robs = fed.getRobs();
  
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  for (const auto& rob : robs) {
    const roddata& rod = rob.getRod();
    const uint32_t& sourceId = rod.getSourceId();
    
    switch(sourceId) {
    case 0X67: // TGC A-Side;
    case 0X68: // TGC C-Side
      //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
      fillTgcRod(rod);
      //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
      break;
    case 0X53: // Tile A-Side
    case 0X54: // Tile C-Side
      //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
      fillTileRod(rod);
      //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
      break;
    default:
      break;
    }
  }
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  if (m_bcidMode) { fillTmdbTgcConbinedInfo(); }
  //if (m_debug) { std::cout << m_algoName << " " << __func__ << " " << __LINE__ << std::endl; }
  m_tree->Fill();
}


void
tree::event_clear() 
{
  nTgcRods = 0;
  nTmdbRods = 0;
  
  // tmdb related variables
  m_tmdb_adc_nsamples.clear();
  m_tmdb_adc_values.clear();
  m_tmdb_energy_values.clear();
  m_tmdb_rodid.clear();
  m_tmdb_rod_bcid.clear();
  m_tmdb_rod_bcid6b.clear();
  m_tmdb_phi.clear();
  m_tmdb_sl_result0.clear();
  m_tmdb_sl_result1.clear();
  m_tmdb_sl_result2.clear();
  m_tmdb_sl_result0_bcid.clear();
  m_tmdb_sl_result1_bcid.clear();
  m_tmdb_sl_result2_bcid.clear();
  
  m_tgcsl_rodid_prev.clear();
  m_tgcsl_rodid_curr.clear();
  m_tgcsl_rodid_next.clear();
  m_tgcsl_rod_bcid_prev.clear();
  m_tgcsl_rod_bcid_curr.clear();
  m_tgcsl_rod_bcid_next.clear();
  m_tgcsl_rod_bcid6b_prev.clear();
  m_tgcsl_rod_bcid6b_curr.clear();
  m_tgcsl_rod_bcid6b_next.clear();
  m_tgcsl_sector_prev.clear();
  m_tgcsl_sector_curr.clear();
  m_tgcsl_sector_next.clear();
  m_tgcsl_isAside_prev.clear();
  m_tgcsl_isAside_curr.clear();
  m_tgcsl_isAside_next.clear();
  m_tgcsl_tmdb_loc_prev.clear();
  m_tgcsl_tmdb_loc_curr.clear();
  m_tgcsl_tmdb_loc_next.clear();
  m_tgcsl_tmdb_tilecalbits_prev.clear();
  m_tgcsl_tmdb_tilecalbits_curr.clear();
  m_tgcsl_tmdb_tilecalbits_next.clear();
  m_tgcsl_tmdb_tilecalbits_bcid_prev.clear();
  m_tgcsl_tmdb_tilecalbits_bcid_curr.clear();
  m_tgcsl_tmdb_tilecalbits_bcid_next.clear();
  
  m_tgcsl_rodid.clear();
  m_tgcsl_sectorid.clear();
  m_tgcsl_isAside.clear();
  m_tgcsl_rod_bcid.clear();
  m_tgcsl_rod_bcid6b.clear();
  m_tgcsl_bctiming.clear();
  m_tgcsl_correspondingtile0_bcid.clear();
  m_tgcsl_correspondingtile1_bcid.clear();
  m_tgcsl_correspondingtile2_bcid.clear();
  m_tgcsl_correspondingtile3_bcid.clear();
  m_tgcsl_correspondingtile0.clear();
  m_tgcsl_correspondingtile1.clear();
  m_tgcsl_correspondingtile2.clear();
  m_tgcsl_correspondingtile3.clear();
  m_tgcsl_tilecalobits0.clear();
  m_tgcsl_tilecalobits1.clear();
  m_tgcsl_tilecalobits2.clear();
  m_tgcsl_tilecalobits3.clear();
  m_tgcsl_tilecalobits0_bcid.clear();
  m_tgcsl_tilecalobits1_bcid.clear();
  m_tgcsl_tilecalobits2_bcid.clear();
  m_tgcsl_tilecalobits3_bcid.clear();


  m_tgcsl_coin_bit0.clear();
  m_tgcsl_coin_bit1.clear();
  m_tgcsl_coin_bit2.clear();
  m_tgcsl_coin_bit3.clear();

}


void 
tree::fillTmdbTgcConbinedInfo()
{ 
  std::map<std::pair<bool, uint32_t>, uint32_t> tmdbSidePhiBcidMap0;
  std::map<std::pair<bool, uint32_t>, uint32_t> tmdbSidePhiBcidMap1;
  std::map<std::pair<bool, uint32_t>, uint32_t> tmdbSidePhiBcidMap2;
  std::map<std::pair<bool, uint32_t>, unsigned long long> tmdbSidePhiResultMap;
  for (uint32_t iTmdb=0, nTmdbs=m_tmdb_rodid.size(); iTmdb<nTmdbs; iTmdb++) {
    std::pair<bool, uint32_t> sidePhiPair(m_tmdb_isAside.at(iTmdb), m_tmdb_phi.at(iTmdb));
    //
    tmdbSidePhiBcidMap0.insert(std::make_pair(sidePhiPair, m_tmdb_sl_result0_bcid.at(iTmdb) ) );
    //modified by higashida 0612
    tmdbSidePhiBcidMap1.insert(std::make_pair(sidePhiPair, m_tmdb_sl_result1_bcid.at(iTmdb) ) );
    tmdbSidePhiBcidMap2.insert(std::make_pair(sidePhiPair, m_tmdb_sl_result2_bcid.at(iTmdb) ) );
    //modified by higashida 0612

    unsigned long long result0 = m_tmdb_sl_result0.at(iTmdb);
    unsigned long long result1 = m_tmdb_sl_result1.at(iTmdb);
    unsigned long long result2 = m_tmdb_sl_result2.at(iTmdb);
    unsigned long long results = ( (result2<<32) | (result1<<16) | (result0) );
    tmdbSidePhiResultMap.insert(std::make_pair(sidePhiPair, results) );
    //printf("%016llx\n", results);
  }
  
  //fill (m_tgcsl_correspondingtile_bcid)
  for (uint32_t iTgcSlTmdbInfo=0, nTgcSlTmdbInfo=m_tgcsl_rodid.size(); 
       iTgcSlTmdbInfo<nTgcSlTmdbInfo; iTgcSlTmdbInfo++) {
    const uint32_t& rodid = m_tgcsl_rodid.at(iTgcSlTmdbInfo);
    const uint32_t tgcSector=(rodid&0XF);
    const bool tgcIsAside=((rodid&0XFF0000)==0X670000);
    const uint32_t correspondingTmdbIdForSlbId0 = getTmdbPhiFromTGCSector(tgcSector, 0);
    const uint32_t correspondingTmdbIdForSlbId1 = getTmdbPhiFromTGCSector(tgcSector, 1);
    const uint32_t correspondingTmdbIdForSlbId2 = getTmdbPhiFromTGCSector(tgcSector, 2);
    const uint32_t correspondingTmdbIdForSlbId3 = getTmdbPhiFromTGCSector(tgcSector, 3);
    int ResultFlag = 0;
    if( tgcSector == 1 || tgcSector == 4 || tgcSector == 7 || tgcSector == 10){
      ResultFlag = 0;
    }    
    if( tgcSector == 2 || tgcSector == 5 || tgcSector == 8 || tgcSector == 11){
      ResultFlag = 1;
    }    
    if( tgcSector == 3 || tgcSector == 6 || tgcSector == 9 || tgcSector == 12){
      ResultFlag = 2;
    }  
    /*  
    if( tgcSector == 12){
      ResultFlag = 3;
    }   
    */
    if(ResultFlag == 1){
      if (tmdbSidePhiBcidMap1.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0))==tmdbSidePhiBcidMap1.end()) { 
	m_tgcsl_correspondingtile0_bcid.push_back(10000);      
	//      std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId0 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile0_bcid.push_back(tmdbSidePhiBcidMap1.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0) ));
      }
      
      if (tmdbSidePhiBcidMap1.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1))==tmdbSidePhiBcidMap1.end()) { 
	m_tgcsl_correspondingtile1_bcid.push_back(10000);      
	//   std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId1 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile1_bcid.push_back(tmdbSidePhiBcidMap1.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1) ));
      }
    
      if (tmdbSidePhiBcidMap2.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2))==tmdbSidePhiBcidMap2.end()) { 
	m_tgcsl_correspondingtile2_bcid.push_back(10000);      
	//  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId2 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile2_bcid.push_back(tmdbSidePhiBcidMap2.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2) ));
      }

      if (tmdbSidePhiBcidMap2.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3))==tmdbSidePhiBcidMap2.end()) { 
	m_tgcsl_correspondingtile3_bcid.push_back(10000);      
	//  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId3 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile3_bcid.push_back(tmdbSidePhiBcidMap2.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3) ));
      }
    }

    if(ResultFlag == 2){
      if (tmdbSidePhiBcidMap0.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0))==tmdbSidePhiBcidMap0.end()) { 
	m_tgcsl_correspondingtile0_bcid.push_back(10000);      
	//      std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId0 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile0_bcid.push_back(tmdbSidePhiBcidMap0.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0) ));
      }

      if (tmdbSidePhiBcidMap0.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1))==tmdbSidePhiBcidMap0.end()) { 
	m_tgcsl_correspondingtile1_bcid.push_back(10000);      
	//   std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId1 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile1_bcid.push_back(tmdbSidePhiBcidMap0.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1) ));
      }
      
      if (tmdbSidePhiBcidMap1.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2))==tmdbSidePhiBcidMap1.end()) { 
	m_tgcsl_correspondingtile2_bcid.push_back(10000);      
	//  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId2 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile2_bcid.push_back(tmdbSidePhiBcidMap1.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2) ));
      }
      
      if (tmdbSidePhiBcidMap1.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3))==tmdbSidePhiBcidMap1.end()) { 
	m_tgcsl_correspondingtile3_bcid.push_back(10000);      
	//  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId3 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile3_bcid.push_back(tmdbSidePhiBcidMap1.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3) ));
      }
    }
    
    if(ResultFlag == 0){
      if (tmdbSidePhiBcidMap2.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0))==tmdbSidePhiBcidMap2.end()) { 
	m_tgcsl_correspondingtile0_bcid.push_back(10000);      
	//      std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId0 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile0_bcid.push_back(tmdbSidePhiBcidMap2.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0) ));
      }
      
      if (tmdbSidePhiBcidMap2.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1))==tmdbSidePhiBcidMap2.end()) { 
	m_tgcsl_correspondingtile1_bcid.push_back(10000);      
	//   std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId1 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile1_bcid.push_back(tmdbSidePhiBcidMap2.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1) ));
      }
      
      if (tmdbSidePhiBcidMap0.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2))==tmdbSidePhiBcidMap0.end()) { 
	m_tgcsl_correspondingtile2_bcid.push_back(10000);      
	//  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId2 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile2_bcid.push_back(tmdbSidePhiBcidMap0.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2) ));
      }
      
      if (tmdbSidePhiBcidMap0.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3))==tmdbSidePhiBcidMap0.end()) { 
	m_tgcsl_correspondingtile3_bcid.push_back(10000);      
	//  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId3 << " is not found" << std::endl;
      } else {
	m_tgcsl_correspondingtile3_bcid.push_back(tmdbSidePhiBcidMap0.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3) ));
      }
    }


    //
    //
    if (tmdbSidePhiResultMap.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0))==tmdbSidePhiResultMap.end()) { 
      m_tgcsl_correspondingtile0.push_back(10000);      
      //  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId0 << " is not found" << std::endl;
    } else {
      m_tgcsl_correspondingtile0.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0) ));
    }
    
    if (tmdbSidePhiResultMap.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1))==tmdbSidePhiResultMap.end()) { 
      m_tgcsl_correspondingtile1.push_back(10000);      
      //   std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId1 << " is not found" << std::endl;
    } else {
      m_tgcsl_correspondingtile1.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1) ));
    }

    if (tmdbSidePhiResultMap.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2))==tmdbSidePhiResultMap.end()) { 
      m_tgcsl_correspondingtile2.push_back(10000);      
      //   std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId2 << " is not found" << std::endl;
    } else {
      m_tgcsl_correspondingtile2.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2) ));
    }

    if (tmdbSidePhiResultMap.find(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3))==tmdbSidePhiResultMap.end()) { 
      m_tgcsl_correspondingtile3.push_back(10000);      
      //  std::cout << __LINE__ << " " << __FILE__ << " tile of " << (tgcIsAside ? "A" : "C") << correspondingTmdbIdForSlbId3 << " is not found" << std::endl;
    } else {
      m_tgcsl_correspondingtile3.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3) ));
    }
    
    
    


    // m_tgcsl_correspondingtile0.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId0) ));
    // m_tgcsl_correspondingtile1.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId1) ));
    // m_tgcsl_correspondingtile2.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId2) ));
    // m_tgcsl_correspondingtile3.push_back(tmdbSidePhiResultMap.at(std::make_pair(tgcIsAside, correspondingTmdbIdForSlbId3) ));
  }
  
}


void 
tree::write()
{
  m_tree->Write();
  m_file->Write();
}
