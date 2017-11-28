// simple standalone decoder

#include <fstream>
#include <iostream>

// standard liibrary for C
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <map>

// my reader software
#include "fileStartRecord.hh"
#include "fileNameStrings.hh"
#include "metadataStrings.hh"
#include "runParametersRecord.hh"
#include "dataSeparatorRecord.hh"
#include "fullEventData.hh"
#include "fileEndRecord.hh"
#include "tileCaloROD.hh"
#include "tgcROD.hh"
#include "tree.hh"
#include "utils.hh"

// ROOT
#include <TROOT.h>

const int AnalysisModeTileSLTest = 1;
void TileSLTestAnalysis(const fullEventData& fed, bool debug);
uint32_t ReconstructTgcslOutFromTileOut(const bool isAside,
					const int sector12,
					const int sbLoc,
					const std::map<std::pair<bool, int>, uint32_t>& moduleResults);

int
main(int argc, char* argv[])
{
  std::string fileName("filename");
  std::string outputFileName("output.root");
  gROOT->ProcessLine(".L loader.C++");
  
  int c;
  int maxeve = -1;
  bool bcidMode = false;
  bool debug = false;
  int specialAnalysisMode = -1;
  //  int specialAnalysisMode = 1;
  
  while ( (c = getopt(argc, argv, "hI:N:O:bdM:") ) !=-1 ) {
    switch (c) {
    case 'h': // help print
      printf("<program> -I <inputfilename> -O <outputfile> -N <events> (-b -M <analysis mode>)\n");
      exit(EXIT_SUCCESS);
    case 'I':
      fileName = optarg;
      break;
    case 'O':
      outputFileName = optarg;
      break;
    case 'N':
      maxeve = strtoul(optarg, NULL, 0);
      break;
    case 'b':
      bcidMode = true;
      break;
    case 'd':
      debug = true;
      break;
    case 'M':
      specialAnalysisMode = strtol(optarg, NULL, 0);
      break;
    default: // default block
      break;
    }  
  }  
  
  std::ifstream inputfile(fileName.c_str());
  if (not inputfile.is_open()) {
    printf("filename is invalid. \n");
    printf("<program> -I <filename> \n");
    exit(EXIT_SUCCESS);    
  }
  
  int nEvents=0;
  
  tree myTree(outputFileName.c_str(), "tree", "tree");
  myTree.setBcidMode(bcidMode);
  myTree.SetDebug(debug);
  myTree.SetName("tree filler");
  
  while (not (inputfile.eof())) {
    // read and decode the file metadata part
    fileStartRecord fsr; 
    fsr.SetDebug(debug); fsr.SetName("File Start Recoder");
    fsr.ProceedFilePointerAndDecode(inputfile); 
    
    fileNameStrings fns;
    fns.SetDebug(debug); fns.SetName("File Name String Recoder");
    fns.ProceedFilePointerAndDecode(inputfile);
    
    metadataStrings mds;
    mds.SetDebug(debug); mds.SetName("Metadata String Recoder");
    mds.ProceedFilePointerAndDecode(inputfile);
    
    runParametersRecord rpr;
    rpr.SetDebug(debug); rpr.SetName("Run Parameter Recorder");
    rpr.ProceedFilePointerAndDecode(inputfile);
    
    // event loop
    while (not (inputfile.eof())) {
      std::ifstream::pos_type currp = inputfile.tellg();
      dataSeparatorRecord dsr;
      dsr.SetDebug(debug); dsr.SetName("Data Separator Recorder");
      dsr.ProceedFilePointerAndDecode(inputfile);
      if (!(dsr.isSeparatorRecord())) {
	inputfile.clear();
	inputfile.seekg(currp); // point back
	break;
      }
      
      if (nEvents==maxeve) {break;}
      nEvents++;
      // added for log
      //      if (nEvents==2){continue;}
      printf("Event = %d \n", nEvents);
      fullEventData fed;
      fed.SetDebug(debug); fed.SetName("Full Event Data Recoder");
      fed.ProceedFilePointerAndDecode(inputfile);
      //printf("debug> event start (decoder.cc)\n");
      myTree.fill(fed);
      
      if (specialAnalysisMode==AnalysisModeTileSLTest) {
	printf("[%dth event]\n", nEvents);
	TileSLTestAnalysis(fed, debug);
      }
      
      if (nEvents%100==0) { printf("decoder :: %d events have been processed.\n", nEvents); }
    }
    
    printf("nEvents=%d processed\n", nEvents);
    if (nEvents==maxeve) {break;} // skip the fer decode
    fileEndRecord fer;
    fer.SetDebug(debug);
    fer.ProceedFilePointerAndDecode(inputfile);
    break;
  }
  
  myTree.write();
}

void TileSLTestAnalysis(const fullEventData& fed, bool debug)
{
  const std::vector<robdata>& robs = fed.getRobs();  
  std::map<uint32_t, tgcROD> tgcs;
  std::map<uint32_t, tileCaloROD> tiles;
  
  std::cout << "[1] ROD raw data for TGC and Tile" << std::endl; // debug
  std::cout << "---------------------------------" << std::endl; // debug
    
  for (const auto& rob : robs) {
    const roddata& rod = rob.getRod();
    const uint32_t& sourceId = rod.getSourceId();
    
    if (sourceId == 0X67 or sourceId == 0X68) {
      tgcROD tgc;
      tgc.Analysis(rod);
      tgcs.insert(std::make_pair(rod.getRodId(), tgc));
      const std::vector<uint32_t>& words = rod.getWords();
      
      if (debug) { for (const auto& word : words) {printf("%08x\n", word);} printf("\n"); } // debug
    } 
    
    if (sourceId == 0X53 or sourceId == 0X54) {
      tileCaloROD tile;
      tile.Analysis(rod);
      if (not tile.withTMDB()) {continue;}
      tiles.insert(std::make_pair(rod.getRodId(), tile));
      const std::vector<uint32_t>& words = rod.getWords();
      
      if (debug) { for (const auto& word : words) {printf("%08x\n", word);} printf("\n"); } // debug
    }
  }
  
   std::cout << std::endl; // for debug purpose
   std::cout << "[2] Tile 48 bits of results" << std::endl; // for debug purpose
   std::cout << "---------------------------" << std::endl; // for debug purpose
  std::map<std::pair<bool, int>, uint32_t> moduleResults;
  
  for (const auto& tile : tiles) {
    printf("%08x  :%s\n", tile.first, utils::getResult48bits( tile.second.getSlResult() ).c_str() );
    int  sector  = tile.second.getSector();
    bool isAside = tile.second.getIsAside();
    
    for (int ii=0; ii<8; ii++) {
      int modId = sector*8 + ii;
      const uint32_t modResult = tile.second.getThrehshold4bits(ii);
      moduleResults.insert(std::make_pair(std::make_pair(isAside, modId), modResult));
    }
  }
  
  std::cout << std::endl;
  std::cout << "8 bits for each TGC 96 trigger sectors" << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  printf("%4s | %8s | %6s | 8-bit\n", "side", "TrigSec", "C/N/P" );
  for (const auto& tgc : tgcs) {
    const std::vector<uint32_t>& bitP = tgc.second.getSlTmdbTilecalbitsPrev();
    const std::vector<uint32_t>& locP = tgc.second.getSlTmdbLocPrev();
    const std::vector<uint32_t>& bitC = tgc.second.getSlTmdbTilecalbitsCurr();
    const std::vector<uint32_t>& locC = tgc.second.getSlTmdbLocCurr();
    const std::vector<uint32_t>& bitN = tgc.second.getSlTmdbTilecalbitsNext();
    const std::vector<uint32_t>& locN = tgc.second.getSlTmdbLocNext();
    
    std::map<uint32_t, uint32_t> tileInfoP;
    std::map<uint32_t, uint32_t> tileInfoC;
    std::map<uint32_t, uint32_t> tileInfoN;
    
    for (int ii=0, n=bitP.size(); ii<n; ii++) { 
      tileInfoP.insert(std::make_pair(locP.at(ii), bitP.at(ii)) );
    }
    for (int ii=0, n=bitC.size(); ii<n; ii++) { 
      if (debug) { printf("%2x [%d] ", bitC.at(ii), locC.at(ii)); }
      tileInfoC.insert(std::make_pair(locC.at(ii), bitC.at(ii)) );
    }
    if (debug) { printf(" : Current \n"); }
    for (int ii=0, n=bitN.size(); ii<n; ii++) { 
      tileInfoN.insert(std::make_pair(locN.at(ii), bitN.at(ii)) );
    }

    // test
     // for (int ii=0; ii<4; ii++) {
     //   int triggerSector = (((tgc.first&0Xf)-1)*4+ii);
     //   if (tileInfoP.find(ii)==tileInfoP.end()) {
     // 	 printf("%08x|%8d|%6s|%s\n", tgc.first, triggerSector, "P", utils::get32bits(0, 8).c_str() );
     //   } else {
     // 	 printf("%08x|%8d|%6s|%s\n", tgc.first, triggerSector, "P", utils::get32bits(tileInfoP.at(ii), 8).c_str() );
     //   }
     // }

    // test
    for (int ii=0; ii<4; ii++) {
      int triggerSector = (((tgc.first&0Xf)-1)*4+ii);
      uint32_t expectation = ReconstructTgcslOutFromTileOut((tgc.second.getSide()=="A"), // isAside
							    tgc.second.getSector(),
							    ii, moduleResults);
      const std::string& side = tgc.second.getSide();
      // test


        if (tileInfoC.find(ii)==tileInfoC.end()) {
        	printf("%4s | %8d | %6s | %s | %s \n", side.c_str(), triggerSector, "C", 
        	       utils::get32bits(0, 8).c_str(), utils::get32bits(expectation, 8).c_str());
        } else {
        	printf("%4s | %8d | %6s | %s | %s \n", side.c_str(), triggerSector, "C", 
        	       utils::get32bits(tileInfoC.at(ii), 8).c_str(), utils::get32bits(expectation, 8).c_str());
        }
      //test
      std::string expected8BitFromTile = utils::get32bits(expectation, 8).c_str();
      std::string P8BitFromSL = ((tileInfoP.find(ii)==tileInfoP.end()) ? utils::get32bits(0, 8) : utils::get32bits(tileInfoP.at(ii), 8));
      std::string C8BitFromSL = ((tileInfoC.find(ii)==tileInfoC.end()) ? utils::get32bits(0, 8) : utils::get32bits(tileInfoC.at(ii), 8));
      std::string N8BitFromSL = ((tileInfoN.find(ii)==tileInfoN.end()) ? utils::get32bits(0, 8) : utils::get32bits(tileInfoN.at(ii), 8));
      if (expectation!=0X0) {
	if (P8BitFromSL==expected8BitFromTile) {P8BitFromSL=std::string("\033[1m\033[32m")+P8BitFromSL+std::string("\033[0m\033[0m");}
	if (C8BitFromSL==expected8BitFromTile) {C8BitFromSL=std::string("\033[1m\033[32m")+C8BitFromSL+std::string("\033[0m\033[0m");}
	if (N8BitFromSL==expected8BitFromTile) {N8BitFromSL=std::string("\033[1m\033[32m")+N8BitFromSL+std::string("\033[0m\033[0m");}
	expected8BitFromTile = std::string("\033[1m\033[31m")+expected8BitFromTile+std::string("\033[0m\033[0m");
      } else {
	if (P8BitFromSL==expected8BitFromTile) {P8BitFromSL=std::string("\033[34m")+P8BitFromSL+std::string("\033[0m");}
	if (C8BitFromSL==expected8BitFromTile) {C8BitFromSL=std::string("\033[34m")+C8BitFromSL+std::string("\033[0m");}
	if (N8BitFromSL==expected8BitFromTile) {N8BitFromSL=std::string("\033[34m")+N8BitFromSL+std::string("\033[0m");}
      }
      
      //      printf("%4s | %8d | %6s | P=%s | C=%s | N=%s | Expectation=%s \n", side.c_str(), triggerSector, "C", 
      //     P8BitFromSL.c_str(), C8BitFromSL.c_str(), N8BitFromSL.c_str(), expected8BitFromTile.c_str());
    }
    
     // for (int ii=0; ii<4; ii++) {
     //   int triggerSector = (((tgc.first&0Xf)-1)*4+ii);
     //   if (tileInfoN.find(ii)==tileInfoN.end()) {
     // 	printf("%08x|%8d|%6s|%s\n", tgc.first, triggerSector, "N", utils::get32bits(0, 8).c_str() );
     //   } else {
     // 	printf("%08x|%8d|%6s|%s\n", tgc.first, triggerSector, "N", utils::get32bits(tileInfoN.at(ii), 8).c_str() );
     //   }
     // }
  }
  std::cout << std::endl;
  
}

uint32_t ReconstructTgcslOutFromTileOut(const bool isAside,
					const int sector12,
					const int sbLoc,
					const std::map<std::pair<bool, int>, uint32_t>& moduleResults)
{
  const int triggerSector = (sector12-1)*2 + (sbLoc-sbLoc%2)/2;
  const int globalGlinkId = ((triggerSector+23)%24);
  const int localGlinkId = (globalGlinkId%3);
  const int tmdbId = (globalGlinkId-localGlinkId)/3;
  const int startModule = tmdbId*8;
  // std::cout << "debug: sector12=" << sector12 << " sbLoc=" << sbLoc 
  // 	    << " triggerSector=" << triggerSector << " globalGlinkId=" << globalGlinkId 
  // 	    << " localGlinkId=" << localGlinkId << " tmdbId=" << tmdbId 
  // 	    << " startModule=" << startModule << std::endl;

  
  int moduleNumbers[4] = {-1, -1, -1, -1};
  
  switch (localGlinkId) {
  case 2:
    moduleNumbers[1] = startModule;
    moduleNumbers[2] = startModule+1;
    moduleNumbers[3] = startModule+2;
    break;
  case 1:
    moduleNumbers[0] = startModule+2;
    moduleNumbers[1] = startModule+3;
    moduleNumbers[2] = startModule+4;
    moduleNumbers[3] = startModule+5;
    break;
  case 0:
    moduleNumbers[0] = startModule+5;
    moduleNumbers[1] = startModule+6;
    moduleNumbers[2] = startModule+7;
    break;
  default:
    std::cout << "decode error" << std::endl;
    break;
  }
  
  uint32_t rc = 0X0;
  for (int ii=0; ii<4; ii++) {
    if (moduleNumbers[ii]<0) {continue;}
    if (moduleResults.find( std::make_pair(isAside, moduleNumbers[ii]) ) == moduleResults.end() ) {
      std::cout << "decode error" << __LINE__ << " " << (isAside? "A":"C") << " " << moduleNumbers[ii] << std::endl;
    }
    uint32_t originalNum = moduleResults.at( std::make_pair(isAside, moduleNumbers[ii]) );
    uint32_t twobit = (((originalNum & 0X4) >> 2) | ((originalNum & 0X1) << 1));
    //std::cout << "debug" << Form("0X%01x (original=0X%01x)", twobit, originalNum) << std::endl;
    rc |= (twobit<<(ii*2));
  }
  return rc;
}
