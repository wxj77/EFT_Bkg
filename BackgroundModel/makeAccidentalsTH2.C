#include <TString.h>
#include <TCanvas.h>
#include <TH2.h>
#include <TRandom3.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
//#include "../Parameters.h"
#define S1MIN 1
#define S1MAX 50
#define NS1BIN 49

#define LOG10S2MIN 2.0
#define LOG10S2MAX 4.0
#define NLOGS2BIN 40


using namespace std;

void makeAccidentalsTH2(int TimeBinNum=1){
  
  int numS1bins = NS1BIN;
  int numlog10S2bins = NLOGS2BIN;
  
  //histogram
  TH2D* hist = new TH2D("Accidentals_S1vslog10S2","Accidentals_S1vslog10S2", NS1BIN, S1MIN, S1MAX, NLOGS2BIN, LOG10S2MIN, LOG10S2MAX); //Wei, the txt file has a fixed s1 s2 bin defination
  
  //grab bin content
  //data is organized by starting in first log10S2 bin, filling the 49 bins of S1, moving to next
  //log10S2 bin, filling, etc
  
  //TString infile_name(TString::Format("fiducialRMax_TB%d.txt",TimeBinNum));
  //fstream infile("/cms26r0/sjh/lux_bkg_model/planA_bkg_model/rmax_tB3.txt");
  ifstream infile( "./bkgModelInputs/run4_randCoincidenceBack_singleLine_final_160701.txt" );
  
  double content;
  int ii_S1 = 1;
  int ii_log10S2 = 1;
  
  while ( infile >> content ){
     
     if(ii_S1==1) cout<<"S1 bin # "<<ii_S1<<", log10S2 bin # "<<ii_log10S2<<", content "<<content<<endl;
     
     hist->SetBinContent( ii_S1, ii_log10S2, content );
     
     if( ii_S1 % numS1bins == 0 ){
        ii_S1 = 1;
        ii_log10S2 ++;
     }else
        ii_S1++;
     
  }
  
  infile.close();
  
  hist->Smooth(1,"k3a");
  
  gStyle->SetOptStat(0);
  hist->Draw("colz");
  
  //cout<<hist->GetBinContent( 23, 56 )<<endl;
  //cout<<hist->GetXaxis()->GetBinCenter(23)<<endl;
  //cout<<hist->GetYaxis()->GetBinCenter(56)<<endl;
  //TString outfile_name(TString::Format("tB%d_fidCut.root",TimeBinNum));
  //TFile *hfile = new TFile("/cms26r0/sjh/lux_bkg_model/planA_bkg_model/tB3_fidCut.root","RECREATE");
  TFile *hfile = new TFile("./PDFs/Bkg_Accidentals_S1Log10S2_TH2F.root","RECREATE");
  hist->Write();
  hfile->Close();
  
  
}
