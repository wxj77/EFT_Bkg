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
class bkgCompTB;
class bkgCompFull;

using namespace std;






//-------each individual time bins.
class bkgComp {
public :
  TString sourceName;
  double expectValue;
  double errValue;
  int timeBin;
  int driftBin;
  TH2F* S1Log10S2;
  TH2F* S1S2;
  TH3F* rphidrift;
  bkgComp();
  bkgComp(TString SourceName, int TB, int DT);
  virtual ~bkgComp();
  virtual void Init(TString sourceName, int TB, int DT);
  virtual bool Notify();
  TString getFileName(TString histogramType, TString sourceName, int timeBin);
};

bkgComp::bkgComp(TString SourceName, int TB, int DT) {

   Init(SourceName, TB, DT);
}

bkgComp::bkgComp() {
  expectValue= 0;
  errValue = 0; 
}


void bkgComp::Init(TString SourceName, int TB, int DT){
  expectValue= 0;
  errValue = 0; 
  sourceName = SourceName.Data();
  // load all histograms and the expected value.
  TString fname;
  TString histogramType;
  timeBin = TB;
  driftBin =DT;

  TString hnn; 
  TString fullhistname;
  
  //--h1
  histogramType = "S1Log10S2_TH2F";
  fname = getFileName(histogramType, sourceName, timeBin);
  TFile* infile = new TFile(fname.Data() ,"read");
  if (infile->IsOpen()){
//  cout<< "open file "<<fname.Data()<< "for source"<<sourceName.Data()<<endl;
  hnn=TString::Format("DT%d",driftBin);
  TH2F* h1 = (TH2F*) infile->Get(hnn.Data());
  fullhistname=TString::Format("S1Log10S2_tb%d_dt%d", timeBin, driftBin);
  S1Log10S2 = (TH2F*) h1->Clone(fullhistname.Data());
  S1Log10S2->SetDirectory(0);
  infile->Close();
  }
  
  //-----------------------------
  
  //--h2
  histogramType = "S1S2_TH2F";
  fname = getFileName(histogramType, sourceName, timeBin);
  TFile* infile2 = new TFile(fname.Data() ,"read");
  if (infile2->IsOpen()){
//  cout<< "open file "<<fname.Data()<< "for source"<<sourceName.Data()<<endl;
  hnn=TString::Format("DT%d",driftBin);
  TH2F* h2 = (TH2F*) infile2->Get(hnn.Data());
  fullhistname=TString::Format("S1S2_tb%d_dt%d", timeBin, driftBin);
  S1S2 = (TH2F*) h2->Clone(fullhistname.Data());
  S1S2->SetDirectory(0);
  infile2->Close();
  }
  
  //-----------------------------  

  //--h3
  histogramType = "rphidrift_TH3F";
  fname = getFileName(histogramType, sourceName, timeBin);
  TFile* infile3 = new TFile(fname.Data() ,"read");
  if (infile3->IsOpen()){
//  cout<< "open file "<<fname.Data()<< "for source"<<sourceName.Data()<<endl;
  hnn=TString::Format("DT%d",driftBin);
  TH3F* h3 = (TH3F*) infile3->Get(hnn.Data());
  fullhistname=TString::Format("rphidrift_tb%d_dt%d", timeBin, driftBin);
  rphidrift = (TH3F*) h3->Clone(fullhistname.Data());
  rphidrift->SetDirectory(0);
  infile3->Close();
  }
  //----------------------------- 
  
  
  else {
    cout<<"no valid input for "<<sourceName.Data()<<endl; exit;
    cout<<"using uniform "<< histogramType.Data() <<"spectrum for "<<sourceName.Data()<<endl;
  }
  //Notify();
}

bool bkgComp::Notify()
{  
//  cout<<"expected value: "<< expectValue <<endl;
//  cout<<"expected value err : "<< errValue <<endl;
  cout<<"entries in S1Log10S2: "<<"\tTB: "<<timeBin<<"\tDT: "<<driftBin<<": "<<S1Log10S2->Integral()<<endl;
  cout<<"Mean value in S1Log10S2: "<<"\tTB: "<<timeBin<<"\tDT: "<<driftBin<<": "<<S1Log10S2->GetMean()<<endl;
  cout<<"entries in S1S2: "<<"\tTB: "<<timeBin<<"\tDT: "<<driftBin<<": "<<S1S2->Integral()<<endl;
  cout<<"Mean value in S1S2: "<<"\tTB: "<<timeBin<<"\tDT: "<<driftBin<<": "<<S1S2->GetMean()<<endl;
  cout<<"entries in rphidrift: "<<"\tTB: "<<timeBin<<"\tDT: "<<driftBin<<": "<<rphidrift->Integral()<<endl;
  cout<<"Mean value in rphidrift: "<<"\tTB: "<<timeBin<<"\tDT: "<<driftBin<<": "<<rphidrift->GetMean()<<endl;
  return 1;
}

bkgComp::~bkgComp()
{
 // cout<<"delete bkgComp: "<<sourceName.Data() <<endl;
}

TString bkgComp::getFileName(TString histogramType, TString SourceName, int TimeBin){
  return TString::Format("../PDFs/Bkg_%s_%s_TB%1d.root", histogramType.Data(), SourceName.Data(),  TimeBin);
}


//-------- one time bin background class.
class bkgCompTB{
public :
  TString sourceName;
  bkgComp bkg_dt[4];
  double expectValue;
  double errValue;
  int timeBin;
  bkgCompTB();
  bkgCompTB(TString SourceName, int TB);
  virtual ~bkgCompTB();
  virtual void Init(TString sourceName, int TB);
};

bkgCompTB::bkgCompTB(){
  expectValue =0;
  errValue = 0;
}

bkgCompTB::bkgCompTB(TString SourceName, int TB){
  Init(SourceName, TB);
}

bkgCompTB::~bkgCompTB()
{
  //  cout<<"delete bkgCompFull: "<<sourceName.Data() <<endl;
}


void bkgCompTB::Init(TString SourceName, int TB){
  expectValue= 0;
  errValue = 0; 
  sourceName =  SourceName;
  timeBin=TB;
  bkg_dt[0].Init(sourceName,  TB, 1);
  bkg_dt[1].Init(sourceName,  TB, 2);
  bkg_dt[2].Init(sourceName,  TB, 3);
  bkg_dt[3].Init(sourceName,  TB, 4);
//  bkg_tb1.Notify();
//  bkg_tb2.Notify();
//  bkg_tb3.Notify();
//  bkg_tb4.Notify();
}




//------- full components with 4 timebins.
class bkgCompFull{
public :
  TString sourceName;
  bkgCompTB bkg_tb[4];
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
   // cout<<"delete bkgCompFull: "<<sourceName.Data() <<endl;
}


void bkgCompFull::Init(TString SourceName){
  expectValue= 0;
  errValue = 0; 
  sourceName =  SourceName;
  bkg_tb[0].Init(sourceName, 1);
  bkg_tb[1].Init(sourceName, 2);
  bkg_tb[2].Init(sourceName, 3);
  bkg_tb[3].Init(sourceName, 4);
//  bkg_tb1.Notify();
//  bkg_tb2.Notify();
//  bkg_tb3.Notify();
//  bkg_tb4.Notify();
}




int bkgClass(){
//  bkgCompFull uniform("Uniform"); //default spectrum with uniform energy and uniform spacial distribution in the detector. 
  bkgCompFull ar37("Ar37");
  bkgCompFull cr37("Ar37");
  ar37.expectValue =1.;
  ar37.errValue =1.;
  cr37.expectValue =2.;
  cout<<ar37.expectValue<<endl;
//  ar37.bkg_tb[2].Notify();
  for(int ii=0; ii<4; ii++){
    for (int jj=0; jj<4; jj++){
      ar37.bkg_tb[ii].bkg_dt[jj].Notify();
      cout<<"\n";
    }
  }
  return 1;
}

#endif











