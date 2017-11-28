#include "tgcROD.hh"

tgcROD::tgcROD() : algoBase() {}

void
tgcROD::Analysis(const roddata& rod)
{
  m_sector = (rod.getRodId() & 0XF);
  
  if (m_debug) { printf( ">> RODID = %08x\n", rod.getRodId() ); }
  const std::vector<uint32_t>& words = rod.getWords();
  const uint32_t nWords = words.size();
  static const uint32_t sizeOfHeaderWords = 9;
  static const uint32_t sizeOfStatusWords = 5;
  
  const uint32_t datasize = words.at(nWords-3);
  uint32_t iWord=sizeOfHeaderWords+sizeOfStatusWords;
  uint32_t nFragments=0;
  uint32_t nFragmentWords=0;
  
  clear();
  
  m_side = (rod.getSourceId() == 0X67) ? "A" : "C";
  
  std::vector<uint32_t> fragmentIDs;
  std::vector<uint32_t> fragmentNWords;
  
  while (true) {
    if (datasize==nFragments+nFragmentWords) { break; }
    const uint32_t& word = words.at(iWord++);
    const uint32_t& fragmentID = ((word&0XFF000000)>>24);
    const uint32_t& fragmentIDWords = ((word&0X00FFFFFF));
    fragmentIDs.push_back(fragmentID);
    fragmentNWords.push_back(fragmentIDWords);
    
    nFragments++;
    nFragmentWords=nFragmentWords+fragmentIDWords;
  }
  
  //for (const auto& word : words) { printf("tgcROD::tgcROD debug %08x\n", word); }
  //printf("tgcROD::tgcROD debug datasize=%u\n", datasize);
  for (uint32_t iFragment=0; iFragment<nFragments; iFragment++) {
    // if (fragmentIDs.at(iFragment)!=8) { // high pT hit
    if (fragmentIDs.at(iFragment)!=8 && fragmentIDs.at(iFragment)!=9 ) { // high pT hit
      iWord+=fragmentNWords.at(iFragment); 
      continue;
    }
    
    // currently only high pT is impelemented (Aug, 2016)
    if(fragmentIDs.at(iFragment) == 8){
      for (uint32_t iHpt=0; 
	   iHpt<fragmentNWords.at(iFragment); 
	   iHpt++) {
	const uint32_t& word = words.at(iWord++);
	highPtReadoutFormat(word);
      }
    }
    
    if(fragmentIDs.at(iFragment) == 9){
      for (uint32_t iSL=0;
	   iSL<fragmentNWords.at(iFragment);
	   iSL++){
	const uint32_t& word = words.at(iWord++);
	SLReadoutFormat(word);
      }
    }
      
  }
  
  // if ( (m_sl_tmdb_loc_prev.size()==m_sl_tmdb_loc_curr.size()) && (m_sl_tmdb_loc_curr.size() == m_sl_tmdb_loc_next.size()) ) {
  //   printf("OK!! prev=%d curr=%d next=%d (0X%08x)\n", (int)m_sl_tmdb_loc_prev.size(), (int)m_sl_tmdb_loc_curr.size(), (int)m_sl_tmdb_loc_next.size(), rod.getRodId());
  // } else {
  //   printf("NG!! prev=%d curr=%d next=%d (0X%08x)\n", (int)m_sl_tmdb_loc_prev.size(), (int)m_sl_tmdb_loc_curr.size(), (int)m_sl_tmdb_loc_next.size(), rod.getRodId());
  // }
}


tgcROD::~tgcROD() {}


void 
tgcROD::highPtReadoutFormat(const uint32_t& word)
{
  const uint32_t type = ((word&0X00038000)>>15);
  const bool isTileCal = (type==0X5);
  const uint32_t& bcmap = ((word&0X001c0000)>>18);
  const uint32_t& tilebits = (word&0X000000FF);
  const uint32_t& loc = ((word&0X00006000)>>13);  
  
  // currently only tilecalo is implemented
  //printf("tgcROD::highPtReadoutFormat() debug> type=%u\n", type);
  if (!isTileCal) { 
    //printf("tgcROD::tgcROD debug %08x\n", word);
    return; 
  }
  
  uint32_t bc=0;
  switch (bcmap) {
  case 0X4:
    m_sl_tmdb_tilecalbits_prev.push_back(tilebits);
    m_sl_tmdb_loc_prev.push_back(loc);
    bc=0;
    break;
  case 0X2:
    m_sl_tmdb_tilecalbits_curr.push_back(tilebits);
    m_sl_tmdb_loc_curr.push_back(loc);
    bc=1;
    break;
  case 0X1:
    m_sl_tmdb_tilecalbits_next.push_back(tilebits);
    m_sl_tmdb_loc_next.push_back(loc);
    bc=2;
    break;	
  default:
    printf("\033[91m tgcROD ERROR unexpected bcmap=%x loc=%d \033[0m\n ", bcmap, loc);
    break;
  }
  if (m_debug) {printf(">> %08x [%s] [%d] tilebits=%02x \n", word, 
		       ( (bcmap==0X4) ? "P" : (bcmap==0X2) ? "C" : (bcmap==0X1) ? "N" : "-" ), 
		       loc, tilebits);}
  
  //printf("tgcROD::tgcROD debug %08x (bc=%d loc=%d) \n", word, bc, loc);
}

// Added by Akihiro Higashida 171002
void
tgcROD::SLReadoutFormat(const uint32_t& word)
{
  const uint32_t type = ((word&0X00200000)>>21);
  const bool isSLout = (type==0X0);
  const uint32_t& bcmap = ((word&0X01c00000)>>22);
  const bool slcoinveto = ((word&04000000)==1);
  const uint32_t& slbit = (word&0XFFFFFFFF);
  const uint32_t& RoI = (word&0X000000FF);
  const uint32_t& Pt = ((word&0X00000d00)>>9);
  const uint32_t& loc = ((word&0X000c0000)>>18);  
  
  if (!isSLout) {
    return;
  } 
  switch (bcmap) {
  case 0X4:
  m_sl_coin_veto_prev.push_back(slcoinveto);
  m_sl_coin_bit_prev.push_back(slbit);
  m_sl_coin_loc_prev.push_back(loc);
  break;
  case 0X2:
  m_sl_coin_veto_curr.push_back(slcoinveto);
  m_sl_coin_bit_curr.push_back(slbit);
  m_sl_coin_loc_curr.push_back(loc);
  break;
  case 0X1:
  m_sl_coin_veto_next.push_back(slcoinveto);
  m_sl_coin_bit_next.push_back(slbit);
  m_sl_coin_loc_next.push_back(loc);
  break;
  default:
    printf("\033[91m tgcROD ERROR unexpected bcmap=%x loc=%d \033[0m\n ", bcmap, loc);
    break;

  }
  

}
// Added by Akihiro Higashida 171002

void 
tgcROD::clear()
{
  m_sl_tmdb_tilecalbits_prev.clear();
  m_sl_tmdb_loc_prev.clear();
  m_sl_tmdb_tilecalbits_curr.clear();
  m_sl_tmdb_loc_curr.clear();
  m_sl_tmdb_tilecalbits_next.clear();
  m_sl_tmdb_loc_next.clear();  
  
  m_sl_coin_veto_prev.clear();
  m_sl_coin_bit_prev.clear();
  m_sl_coin_loc_prev.clear();
  m_sl_coin_veto_curr.clear();
  m_sl_coin_bit_curr.clear();
  m_sl_coin_loc_curr.clear();
  m_sl_coin_veto_next.clear();
  m_sl_coin_bit_next.clear();
  m_sl_coin_loc_next.clear();
  
}
