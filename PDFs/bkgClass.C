// parameter for background model
#ifndef BKGCLASS_C
#define BKGCLASS_C 1


//c lib
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <string.h>

//root lib
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TMath.h"
#include "TCut.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TProfile.h"
#include "TF1.h"


//custom libs
class bkgComp;
class bkgCompFull;

using namespace std;






//-------each individual time bins.
class bkgComp {
public :
  TString sourceName;
  double expectValue;
  double errValue;
  int timeBin;
  TH2F* S1Log10S2_DT1;
  TH2F* S1Log10S2_DT2;
  TH2F* S1Log10S2_DT3;
  TH2F* S1Log10S2_DT4;
  TH2F* S1S2_DT1;
  TH2F* S1S2_DT2;
  TH2F* S1S2_DT3;
  TH2F* S1S2_DT4;
  TH3F* rphidrift_DT1;
  TH3F* rphidrift_DT2;
  TH3F* rphidrift_DT3;
  TH3F* rphidrift_DT4;
  bkgComp();
  bkgComp(TString SourceName, int TB);
  virtual ~bkgComp();
  virtual void Init(TString sourceName, int TB);
  virtual bool Notify();
  TString getFileName(TString histogramType, TString sourceName, int timeBin);
};

bkgComp::bkgComp(TString SourceName, int TB) {

   Init(SourceName, TB);
}

bkgComp::bkgComp() {
  expectValue= 0;
  errValue = 0; 
}


void bkgComp::Init(TString SourceName, int TB){
  expectValue= 0;
  errValue = 0; 
  sourceName = SourceName.Data();
  // load all histograms and the expected value.
  TString fname;
  TString histogramType;
  timeBin = TB;

  histogramType = "S1Log10S2_TH2F"; 
  fname = getFileName(histogramType, sourceName, timeBin);
  cout<<fname.Data()<<endl;
  TFile* infile = new TFile(fname.Data() ,"read");
  if (infile->IsOpen()){
  cout<< "open file "<<fname.Data()<< "for source"<<sourceName.Data()<<endl;
  TH2F* DT1 = (TH2F*) infile->Get("DT1");
  S1Log10S2_DT1 = (TH2F*) DT1->Clone();
  TH2F* DT2 = (TH2F*) infile->Get("DT2");
  S1Log10S2_DT2 = (TH2F*) DT2->Clone();
  TH2F* DT3 = (TH2F*) infile->Get("DT3");
  S1Log10S2_DT3 = (TH2F*) DT3->Clone();
  TH2F* DT4 = (TH2F*) infile->Get("DT4");
  S1Log10S2_DT4 = (TH2F*) DT4->Clone();
  }
  else {
    cout<<"no valid input for "<<sourceName.Data()<<endl;
    cout<<"using uniform "<< histogramType.Data() <<"spectrum for "<<sourceName.Data()<<endl;
    fname = getFileName(histogramType, "Uniform",  timeBin);
  cout<<fname.Data()<<endl;
  TFile* infile= new TFile(fname.Data(), "read");
  TH2F* DT1 = (TH2F*) infile->Get("DT1");
  S1Log10S2_DT1 = (TH2F*) DT1->Clone();
  TH2F* DT2 = (TH2F*) infile->Get("DT2");
  S1Log10S2_DT2 = (TH2F*) DT2->Clone();
  TH2F* DT3 = (TH2F*) infile->Get("DT3");
  S1Log10S2_DT3 = (TH2F*) DT3->Clone();
  TH2F* DT4 = (TH2F*) infile->Get("DT4");
  S1Log10S2_DT4 = (TH2F*) DT4->Clone();    
  }
  Notify();
}

bool bkgComp::Notify()
{  
//  cout<<"expected value: "<< expectValue <<endl;
//  cout<<"expected value err : "<< errValue <<endl;
  cout<<"entries in S1Log10S2: "<<"\tDT1: "<<S1Log10S2_DT1->Integral()<<"\tDT2: "<<S1Log10S2_DT2->Integral()<<"\tDT3: "<<S1Log10S2_DT3->Integral()<<"\tDT4: "<<S1Log10S2_DT4->Integral() <<endl;
  cout<<"Mean value in S1Log10S2: "<<"\tDT1: "<<S1Log10S2_DT1->GetMean()<<"\tDT2: "<<S1Log10S2_DT2->GetMean()<<"\tDT3: "<<S1Log10S2_DT3->GetMean()<<"\tDT4: "<<S1Log10S2_DT4->GetMean() <<endl;
  return 1;
}

bkgComp::~bkgComp()
{
  cout<<"delete bkgComp: "<<sourceName.Data() <<endl;
}

TString bkgComp::getFileName(TString histogramType, TString SourceName, int TimeBin){
  return TString::Format("Bkg_%s_%s_TB%1d.root", histogramType.Data(), SourceName.Data(),  TimeBin);
}








//------- full components with 4 timebins.
class bkgCompFull{
public :
  TString sourceName;
  bkgComp bkg_tb1;
  bkgComp bkg_tb2;
  bkgComp bkg_tb3;
  bkgComp bkg_tb4;
  double expectValue;
  double errValue;
  bkgCompFull();
  bkgCompFull(TString SourceName);
  virtual ~bkgCompFull();
  virtual void Init(TString sourceName);
};

bkgCompFull::bkgCompFull(){
  expectValue =0;
  errValue = 0;
}

bkgCompFull::bkgCompFull(TString SourceName){
  Init(SourceName);
}

bkgCompFull::~bkgCompFull()
{
    cout<<"delete bkgCompFull: "<<sourceName.Data() <<endl;
}


void bkgCompFull::Init(TString SourceName){
  expectValue= 0;
  errValue = 0; 
  sourceName =  SourceName;
  bkg_tb1.Init(sourceName, 1);
  bkg_tb2.Init(sourceName, 2);
  bkg_tb3.Init(sourceName, 3);
  bkg_tb4.Init(sourceName, 4);
//  bkg_tb1.Notify();
//  bkg_tb2.Notify();
//  bkg_tb3.Notify();
//  bkg_tb4.Notify();
}

#endif











