#include "fileEndRecord.hh"

fileEndRecord::fileEndRecord() : algoBase() {}

void 
fileEndRecord::ProceedFilePointerAndDecodeUser(std::ifstream& inputfile)
{
  uint32_t bufInt;
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_marker = bufInt;
  //printf("m_marker=%08x\n", m_marker);
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_record_size = bufInt;

  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_data = bufInt;

  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_time = bufInt;

  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_events_in_file = bufInt;
  printf("fileEndRecord::m_events_in_file=%u\n", m_events_in_file);
  
  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_data_in_file = bufInt;

  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_events_in_run = bufInt;

  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_data_in_run = bufInt;

  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_status = bufInt;

  inputfile.read (reinterpret_cast<char *>(&bufInt), sizeof (bufInt));
  m_end_marker = bufInt;
  //printf("m_end_marker=%08x\n", m_end_marker);
}

fileEndRecord::~fileEndRecord() {}
