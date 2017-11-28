void draw()
{
  TFile* f = TFile::Open("output.root");
  TTree* tree = (TTree*)f->Get("tree");
  TLatex latex;
  latex.SetNDC();
  
  std::string ReqAside="((tgcsl_rodid&0xFF0000)==0X670000)";
  std::string ReqCside="((tgcsl_rodid&0xFF0000)==0X680000)";
  
  int iSide=0;
  int iSector=7;

  TCanvas c1;
  c1.Print("output.pdf[");
  TH1D h1("h1", "", 127, -63.5, 63.5); h1.GetXaxis()->SetTitle("#Delta BCID");
  for (int iSide=0; iSide<2; iSide++) {
    for (int iSector=1; iSector<=12; iSector++) {
      tree->Draw("tgcsl_tilecalobits0_bcid-tgcsl_correspondingtile0_bcid>>h1", Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits0_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f", (iSide==0 ? "A" : "C"), iSector, 0, h1.GetBinCenter(h1.GetMaximumBin())) ); c1.Print("output.pdf");
      tree->Draw("tgcsl_tilecalobits1_bcid-tgcsl_correspondingtile1_bcid>>h1", Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits1_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f", (iSide==0 ? "A" : "C"), iSector, 1, h1.GetBinCenter(h1.GetMaximumBin())) ); c1.Print("output.pdf");
      tree->Draw("tgcsl_tilecalobits2_bcid-tgcsl_correspondingtile2_bcid>>h1", Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits2_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f", (iSide==0 ? "A" : "C"), iSector, 2, h1.GetBinCenter(h1.GetMaximumBin())) ); c1.Print("output.pdf");
      tree->Draw("tgcsl_tilecalobits3_bcid-tgcsl_correspondingtile3_bcid>>h1", Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits3_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f", (iSide==0 ? "A" : "C"), iSector, 3, h1.GetBinCenter(h1.GetMaximumBin())) ); c1.Print("output.pdf");
    }
  }
  
  gStyle->SetOptStat(0);
  TH1D h2("h2", "", 64, -0.5, 63.5); h1.GetXaxis()->SetTitle("#Delta BCID");
  TGraphErrors graA; graA.SetTitle("A side");
  TGraphErrors graC ;graC.SetTitle("C side");
  TGraphErrors graANeg; graANeg.SetTitle("A side");
  TGraphErrors graCNeg; graCNeg.SetTitle("C side");
  TGraphErrors* gras[]={&graA, &graC};
  TGraphErrors* grasNeg[]={&graANeg, &graCNeg};

  TGraphErrors graACorr; graA.SetTitle("A side");
  TGraphErrors graCCorr; graC.SetTitle("C side");
  TGraphErrors graANegCorr; graANeg.SetTitle("A side");
  TGraphErrors graCNegCorr; graCNeg.SetTitle("C side");
  TGraphErrors* grasCorr[]={&graACorr, &graCCorr};
  TGraphErrors* grasNegCorr[]={&graANegCorr, &graCNegCorr};
  
  double max, rms;
  // SL-A ( 47 49 51 52 53 54 54 53 52 51 49 49) SL-C ( 46 48 49 51 52 54 54 53 52 51 49 48)
  std::vector<uint32_t> deltaDepthA = {1, 3, 5, 6, 7, 8, 8, 7, 6, 5, 3, 3}; // wrt 46
  std::vector<uint32_t> deltaDepthC = {0, 2, 3, 5, 6, 8, 8, 7, 6, 5, 3, 2}; // wrt 46
  std::vector<std::vector<uint32_t> > deltaDepths = {deltaDepthA, deltaDepthC};
  
  for (int iSide=0; iSide<2; iSide++) {
    int iPoint=0;
    for (int iSector=1; iSector<=12; iSector++) {
      int slbId=0;
      //tree->Draw("(tgcsl_tilecalobits0_bcid-tgcsl_correspondingtile0_bcid+64)%64>>h2", 
      tree->Draw("(tgcsl_tilecalobits0_bcid-tgcsl_rod_bcid6b+64)%64>>h2", 
		 Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits0_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      max=h2.GetBinCenter(h2.GetMaximumBin());      
      rms=h2.GetRMS();
      gras[iSide]->SetPoint(iPoint, iPoint, max); grasNeg[iSide]->SetPoint(iPoint, iPoint, max-64);
      gras[iSide]->SetPointError(iPoint, 0, rms); grasNeg[iSide]->SetPointError(iPoint, 0, rms);
      grasCorr[iSide]->SetPoint(iPoint, iPoint, max+deltaDepths.at(iSide).at(iSector-1)-8); grasNegCorr[iSide]->SetPoint(iPoint, iPoint, max-64+deltaDepths.at(iSide).at(iSector-1)-8);
      grasCorr[iSide]->SetPointError(iPoint, 0, rms); grasNegCorr[iSide]->SetPointError(iPoint, 0, rms);
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f (%.0f)", (iSide==0 ? "A" : "C"), iSector, slbId++, max, max-64) ); c1.Print("output.pdf");
      iPoint++;
      
      //tree->Draw("(tgcsl_tilecalobits1_bcid-tgcsl_correspondingtile1_bcid+64)%64>>h2", 
      tree->Draw("(tgcsl_tilecalobits1_bcid-tgcsl_rod_bcid6b+64)%64>>h2", 
		 Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits1_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      max=h2.GetBinCenter(h2.GetMaximumBin());
      rms=h2.GetRMS();
      gras[iSide]->SetPoint(iPoint, iPoint, max); grasNeg[iSide]->SetPoint(iPoint, iPoint, max-64);
      gras[iSide]->SetPointError(iPoint, 0, rms); grasNeg[iSide]->SetPointError(iPoint, 0, rms);
      grasCorr[iSide]->SetPoint(iPoint, iPoint, max+deltaDepths.at(iSide).at(iSector-1)-8); grasNegCorr[iSide]->SetPoint(iPoint, iPoint, max-64+deltaDepths.at(iSide).at(iSector-1)-8);
      grasCorr[iSide]->SetPointError(iPoint, 0, rms); grasNegCorr[iSide]->SetPointError(iPoint, 0, rms);
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f (%.0f)", (iSide==0 ? "A" : "C"), iSector, slbId++, max, max-64) ); c1.Print("output.pdf");
      iPoint++;
      
      //tree->Draw("(tgcsl_tilecalobits2_bcid-tgcsl_correspondingtile2_bcid+64)%64>>h2", 
      tree->Draw("(tgcsl_tilecalobits2_bcid-tgcsl_rod_bcid6b+64)%64>>h2", 
		 Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits2_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      max=h2.GetBinCenter(h2.GetMaximumBin());
      rms=h2.GetRMS();
      gras[iSide]->SetPoint(iPoint, iPoint, max); grasNeg[iSide]->SetPoint(iPoint, iPoint, max-64);
      gras[iSide]->SetPointError(iPoint, 0, rms); grasNeg[iSide]->SetPointError(iPoint, 0, rms);
      grasCorr[iSide]->SetPoint(iPoint, iPoint, max+deltaDepths.at(iSide).at(iSector-1)-8); grasNegCorr[iSide]->SetPoint(iPoint, iPoint, max-64+deltaDepths.at(iSide).at(iSector-1)-8);
      grasCorr[iSide]->SetPointError(iPoint, 0, rms); grasNegCorr[iSide]->SetPointError(iPoint, 0, rms);
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f (%.0f)", (iSide==0 ? "A" : "C"), iSector, slbId++, max, max-64) ); c1.Print("output.pdf");
      iPoint++;
      
      //tree->Draw("(tgcsl_tilecalobits3_bcid-tgcsl_correspondingtile3_bcid+64)%64>>h2", 
      tree->Draw("(tgcsl_tilecalobits3_bcid-tgcsl_rod_bcid6b+64)%64>>h2", 
		 Form("tgcsl_bctiming==1 && (tgcsl_rodid&0xF)==%d && %s && tgcsl_tilecalobits3_bcid<0X100", iSector, (iSide==0 ? ReqAside.c_str() : ReqCside.c_str() )));
      max=h2.GetBinCenter(h2.GetMaximumBin());
      rms=h2.GetRMS();
      gras[iSide]->SetPoint(iPoint, iPoint, max); grasNeg[iSide]->SetPoint(iPoint, iPoint, max-64);
      gras[iSide]->SetPointError(iPoint, 0, rms); grasNeg[iSide]->SetPointError(iPoint, 0, rms);
      grasCorr[iSide]->SetPoint(iPoint, iPoint, max+deltaDepths.at(iSide).at(iSector-1)-8); grasNegCorr[iSide]->SetPoint(iPoint, iPoint, max-64+deltaDepths.at(iSide).at(iSector-1)-8);
      grasCorr[iSide]->SetPointError(iPoint, 0, rms); grasNegCorr[iSide]->SetPointError(iPoint, 0, rms);
      latex.DrawLatex(0.15,0.9, Form("%s-side Sector%d SLBID=%d peaked at %.0f (%.0f)", (iSide==0 ? "A" : "C"), iSector, slbId++, max, max-64) ); c1.Print("output.pdf");
      iPoint++;
    }
  }

  std::vector<TLine> linesPos, linesNeg;
  for (int ii=0; ii<12; ii++) {
    TLine linePos(ii*4-0.5, 0, ii*4-0.5, 64);
    TLine lineNeg(ii*4-0.5, -64, ii*4-0.5, 0);
    linePos.SetLineStyle(2);
    lineNeg.SetLineStyle(2);
    linesPos.push_back(linePos);
    linesNeg.push_back(lineNeg);
  }
  
  for (int iSide=0; iSide<2; iSide++) {
    gras[iSide]->Draw("APE"); 
    gras[iSide]->GetXaxis()->SetRangeUser(-0.5, 47.5);
    gras[iSide]->SetMaximum(64); gras[iSide]->SetMinimum(0);
    gras[iSide]->SetMarkerStyle(8);
    gras[iSide]->GetXaxis()->SetTitle("(Sector-1) #times 12 + SlbID (peak position & RMS)");
    gras[iSide]->GetYaxis()->SetTitleSize(0.035);
    gras[iSide]->GetYaxis()->SetTitle("SL 6bit-BCID - TMDB 6bit-BCID (=needed delay value)");
    
    grasNeg[iSide]->Draw("APE"); 
    grasNeg[iSide]->GetXaxis()->SetRangeUser(-0.5, 47.5);
    grasNeg[iSide]->SetMaximum(0); grasNeg[iSide]->SetMinimum(-64);
    grasNeg[iSide]->SetMarkerStyle(8);
    grasNeg[iSide]->GetXaxis()->SetTitle("(Sector-1) #times 12 + SlbID (peak position & RMS)");
    grasNeg[iSide]->GetYaxis()->SetTitleSize(0.035);
    grasNeg[iSide]->GetYaxis()->SetTitle("SL 6bit-BCID - TMDB 6bit-BCID (=needed delay value)");
    
    gras[iSide]->Draw("APE"); 
    for (auto& line : linesPos) {line.Draw();}
    c1.Print("output.pdf");
    
    grasNeg[iSide]->Draw("APE"); 
    for (auto& line : linesNeg) {line.Draw();}
    c1.Print("output.pdf");
  }

  for (int iSide=0; iSide<2; iSide++) {
    grasCorr[iSide]->Draw("AP"); 
    grasCorr[iSide]->GetXaxis()->SetRangeUser(-0.5, 47.5);
    grasCorr[iSide]->SetMaximum(64); grasCorr[iSide]->SetMinimum(0);
    grasCorr[iSide]->SetMarkerStyle(8);
    grasCorr[iSide]->GetYaxis()->SetTitleSize(0.030);
    grasCorr[iSide]->GetXaxis()->SetTitle("(Sector-1) #times 12 + SlbID (peak position & RMS)");
    grasCorr[iSide]->GetYaxis()->SetTitle("SL From-TMDB-BCID - SL-ROD BCID (SL-CTP latency corrected)");
    
    grasNegCorr[iSide]->Draw("APE"); 
    grasNegCorr[iSide]->GetXaxis()->SetRangeUser(-0.5, 47.5);
    grasNegCorr[iSide]->SetMaximum(0); grasNegCorr[iSide]->SetMinimum(-64);
    grasNegCorr[iSide]->SetMarkerStyle(8);
    grasNegCorr[iSide]->GetYaxis()->SetTitleSize(0.030);
    grasNegCorr[iSide]->GetXaxis()->SetTitle("(Sector-1) #times 12 + SlbID (peak position & RMS)");
    grasNegCorr[iSide]->GetYaxis()->SetTitle("SL From-TMDB-BCID - SL-ROD BCID (SL-CTP latency corrected)");
    
    grasCorr[iSide]->Draw("APE"); 
    for (auto& line : linesPos) {line.Draw();}
    c1.Print("output.pdf");
    
    grasNegCorr[iSide]->Draw("APE"); 
    for (auto& line : linesNeg) {line.Draw();}
    c1.Print("output.pdf");
  }
  
  c1.Print("output.pdf]");
}
