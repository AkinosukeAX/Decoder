#include "roddata.hh" 

roddata::roddata(int nwords) : algoBase() { m_nwords = nwords; }

void roddata::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile, uint32_t& wordCounter) 
{
  uint32_t bufInt;
  
  // header 0
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_marker = bufInt;
  m_data.push_back(bufInt);
  
  // header 1
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_header_size = bufInt;
  m_data.push_back(bufInt);

  // header 2
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_atlas_format_version = ( (bufInt & 0XFFFF0000) >> 16);
  m_data.push_back(bufInt);
  
  // header 3 
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_rod_id = bufInt;
  m_source_id  = ( (bufInt & 0X00FF0000) >> 16 ) ;
  m_segment_id = ( (bufInt & 0X0000FFFF) ) ;
  m_data.push_back(bufInt);
  
  // header 4
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_run_type   = ( (bufInt & 0XFF000000) >> 24 ) ;
  m_run_number = ( (bufInt & 0X00FFFFFF) ) ;
  m_data.push_back(bufInt);
  
  // header 5
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_level1_id = bufInt;
  m_data.push_back(bufInt);

  // header 6
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_bcid = ( (bufInt & 0X0000FFFF) );
  m_data.push_back(bufInt);
  
  // header 7
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_trigger_type = ( (bufInt & 0X000000FF) );
  m_data.push_back(bufInt);
  
  // header 8
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_detector_event_type = ( (bufInt & 0X00FF0000) >> 16 );
  m_data.push_back(bufInt);
  
  // data word & trailer word
  int nwords = 9; // 9 header words
  wordCounter=wordCounter+9;
  
  while (true) {
    nwords++;
    inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
    wordCounter++;
    m_data.push_back(bufInt);
    if (nwords==m_nwords) {
      if (m_debug) {
	std::ifstream::pos_type currp = inputfile.tellg();
	inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
	printf("roddata nextword=%08x nwords=%u, m_nwords=%u\n", 
	       bufInt, nwords, m_nwords);
	inputfile.clear();
	inputfile.seekg(currp);
      }
      break;    
    }
  }
}

roddata::~roddata() {}
