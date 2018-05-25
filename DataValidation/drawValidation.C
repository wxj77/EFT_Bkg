// how to run
//root -q -l -b "drawValidation.C(\"/home/wxj/EFT_Bkg/DataValidation/eft_bg_data_trial_05182018.root\",1, 1)"

#ifndef DRAWVALIDATION_C
#define DRAWVALIDATION_C 1


//c lib
#include <string>
#include <cstring>
#include <vector>
#include <math.h>
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
#include "TGraph.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TColor.h"
#include "TLegend.h"

//custom libs
#ifndef platinum_cxx
#include "platinum.C"
#endif
//class bkgComp;
//class bkgCompFull;

using namespace std;


bool drawer=1;
bool drawnr=1;
bool drawolder=1;
bool drawoldnr=1;

int drawValidation(TString datafile= "/home/wxj/EFT_Bkg/DataValidation/eft_bg_data_trial_05182018.root",int timeBin=1, int driftBin=1){
int nPoints=0; 
double s1s[100000];
double log10s2s[100000];

TFile* _file0 = new TFile(datafile.Data(), "read");
TTree* tplatinum = (TTree* ) _file0->Get("platinum");
platinum e(tplatinum);

int nentries = tplatinum->GetEntries();
for (int ii=0; ii<nentries; ii++){
  tplatinum->GetEntry(ii);
  bool intb = (e.timeBin==timeBin);
  bool indt = (40.+(driftBin-1.)*65.<=e.drift) && (e.drift<40.+(driftBin)*65.);
  if (intb && indt) {s1s[nPoints]=e.S1;log10s2s[nPoints]=e.log10S2; nPoints++;}  
}


  
  int WhichStyle =4;
  TStyle *lzStyle = new TStyle("lzStyle","LZ Style");

  Int_t FontStyle = 42;
  Float_t FontSizeLabel = 0.04;
  Float_t FontSizeTitle = 0.035;
  Float_t YOffsetTitle = 1.6;
 
  switch(WhichStyle) {
  case 1:
    FontStyle = 42;
    FontSizeLabel = 0.05;
    FontSizeTitle = 0.065;
    YOffsetTitle = 1.19;
    break;
  case 2:
    FontStyle = 42;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  case 3:
    FontStyle = 132;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  }

  lzStyle->SetFrameBorderMode(0);
  lzStyle->SetCanvasBorderMode(0);
  lzStyle->SetPadBorderMode(0);
  lzStyle->SetCanvasBorderSize(0);
  lzStyle->SetFrameBorderSize(0);
  lzStyle->SetDrawBorder(0);
  lzStyle->SetTitleBorderSize(0);

  lzStyle->SetPadColor(0);
  lzStyle->SetCanvasColor(0);
  lzStyle->SetStatColor(0);
  lzStyle->SetFillColor(0);

  lzStyle->SetEndErrorSize(4);
  lzStyle->SetStripDecimals(kFALSE);

  lzStyle->SetLegendBorderSize(0);
  lzStyle->SetLegendFont(FontStyle);

  // set the paper & margin sizes
  lzStyle->SetPaperSize(20, 26);
  lzStyle->SetPadTopMargin(0.2);
  lzStyle->SetPadBottomMargin(0.15);
  lzStyle->SetPadRightMargin(0.13); // 0.075 -> 0.13 for colz option
  lzStyle->SetPadLeftMargin(0.16);//to include both large/small font options
  
  // Fonts, sizes, offsets
  lzStyle->SetTextFont(FontStyle);
  lzStyle->SetTextSize(0.08);

  lzStyle->SetLabelFont(FontStyle, "x");
  lzStyle->SetLabelFont(FontStyle, "y");
  lzStyle->SetLabelFont(FontStyle, "z");
  lzStyle->SetLabelFont(FontStyle, "t");
  lzStyle->SetLabelSize(FontSizeLabel, "x");
  lzStyle->SetLabelSize(FontSizeLabel, "y");
  lzStyle->SetLabelSize(FontSizeLabel, "z");
  lzStyle->SetLabelOffset(0.015, "x");
  lzStyle->SetLabelOffset(0.015, "y");
  lzStyle->SetLabelOffset(0.015, "z");

  lzStyle->SetTitleFont(FontStyle, "x");
  lzStyle->SetTitleFont(FontStyle, "y");
  lzStyle->SetTitleFont(FontStyle, "z");
  lzStyle->SetTitleFont(FontStyle, "t");
  lzStyle->SetTitleSize(FontSizeTitle, "y");
  lzStyle->SetTitleSize(FontSizeTitle, "x");
  lzStyle->SetTitleSize(FontSizeTitle, "z");
  lzStyle->SetTitleOffset(1.14, "x");
  lzStyle->SetTitleOffset(YOffsetTitle, "y");
  lzStyle->SetTitleOffset(1.2, "z");

  lzStyle->SetTitleStyle(0);
  lzStyle->SetTitleFontSize(0.06);//0.08
  lzStyle->SetTitleFont(FontStyle, "pad");
  lzStyle->SetTitleBorderSize(0);
  lzStyle->SetTitleX(0.1f);
  lzStyle->SetTitleW(0.8f);

  // use bold lines and markers
  lzStyle->SetMarkerStyle(1);
  lzStyle->SetHistLineWidth( Width_t(2.5) );
  lzStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes
  
  // get rid of X error bars and y error bar caps
  lzStyle->SetErrorX(0.001);
  
  // do not display any of the standard histogram decorations
  lzStyle->SetOptTitle(0);
  lzStyle->SetOptStat(0);
  lzStyle->SetOptFit(0);
  
  // put tick marks on top and RHS of plots
  lzStyle->SetPadTickX(1);
  lzStyle->SetPadTickY(1);
  
  // -- color --
  // functions blue
  lzStyle->SetFuncColor(600-4);

//  lzStyle->SetFillColor(1); // make color fillings (not white)
  // - color setup for 2D -
  // - "cold"/ blue-ish -
//  double red[]   = { 0.00, 0.09, 0.18, 0.09, 0.00 };
//  double green[] = { 0.01, 0.02, 0.39, 0.68, 0.97 };
//  double blue[]  = { 0.17, 0.39, 0.62, 0.79, 0.97 };
//  double stops[] = { 0.00, 0.02, 0.20, 0.60, 1.00 };
//  const Int_t NRGBs = 5;
  // - "warm" red-ish colors -
  //  double red[]   = {1.00, 1.00, 0.25 };
  //  double green[] = {1.00, 0.00, 0.00 };
  //  double blue[]  = {0.00, 0.00, 0.00 };
  //-grey inverted
    double red[]   = {1.00, 0.00 };
    double green[] = {1.00, 0.00};
    double blue[]  = {1.00, 0.00};
    double stops[]  = {0.00, 1.00};
const Int_t NRGBs = 2;
  

  const Int_t NCont = 255;

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  lzStyle->SetNumberContours(NCont);

/*
const int ncol=10;
double red[ncol];
double green[ncol];
double blue[ncol];
double stops[ncol];

double dcol = -.1/double(ncol);
double gray = 1;
for (int j = 0; j < ncol; j++) {
//   ...... Define color with RGB equal to : gray, gray, gray .......

   red[j]=gray;
   blue[j]=gray;
   green[j]=gray;
   stops[j]=double(j)/double(ncol-1);
   gray += dcol;
}
const int totcol=50;

TColor::CreateGradientColorTable(ncol, red,green,blue,stops,totcol);
lzStyle->SetNumberContours(totcol);*/

  // - Rainbow -
  //  lzStyle->SetPalette(1);  // use the rainbow color set
  //   lzStyle->SetPalette(52); //grey
  //https://root.cern.ch/doc/master/classTColor.html#TColor:SetPalette High quality predefined palettes


  // -- axis --
  lzStyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
  //  TGaxis::SetMaxDigits(3); // doesn't have an effect 
  // no supressed zeroes!
  lzStyle->SetHistMinimumZero(kTRUE);
  


 
  gROOT->SetStyle("lzStyle");
  gROOT->ForceStyle(kTRUE);

  //---Finish setting plot style
  //---start do calculation of energy spectrum.


TCanvas* c1 = new TCanvas("c1",""); //,10,20,200,400

TGraph* datapoints = new TGraph(nPoints, s1s, log10s2s);
datapoints->GetYaxis()->SetRangeUser(2.3,4.5);
datapoints->GetXaxis()->SetRangeUser(1.,300.);
datapoints->SetMarkerStyle(7);
datapoints->SetMarkerColor(kBlack);
datapoints->SetMarkerSize(.8);
datapoints->SetLineStyle(0);
datapoints->SetName("datapoints");
datapoints->SetTitle ("; s1c [phd]; log_{10} s2c [phd]");
datapoints->SetFillColor(0);
datapoints->Draw("AP");


//-----draw ER NR band
  double width=3.0;
  long long iLine=-1,iEvent=0;
  if(drawer){
  int ner=-1;
  double ers1[500];
  double er10[500];
  double er50[500];
  double er90[500];
  TString ERfile = TString::Format("/home/wxj/EFT_Bkg/DataValidation/ER_TB%d_DT%d.txt",timeBin, driftBin );
  cout<<ERfile.Data()<<endl;
  std::string line;
  ifstream infile(ERfile.Data());
  if(!infile){
    cout<<"Invalid Input File!!!!!\n";
  }

  //skip first two lines
  std::string readline;	
  //read dat file
  while(!infile.eof()){
    iLine++;
    ner++; 
    //		if( iLine % 1000 == 0 ) {cout<<"Reading line "<<iLine<<endl;}
    std::getline(infile, readline);
    std::istringstream iss(readline);
    if(!(iss>>ers1[ner]>>er10[ner]>>er50[ner]>>er90[ner])){
      iLine++; cout<<"error/end: "<<endl; std::getline(infile, readline); cout<<readline<<endl; continue;}	
  }
  infile.close();

for (int ii=0; ii<ner; ii++){
  er10[ii]=log10(er10[ii]);
  er50[ii]=log10(er50[ii]);
  er90[ii]=log10(er90[ii]);
//  cout<<er50[ii]<<endl;
}
TGraph* ger50 = new TGraph(ner, ers1, er50);
ger50->SetLineStyle(1);
ger50->SetLineColor(kBlue-7);
ger50->SetLineWidth(width);
ger50->SetName("ger50");
ger50->SetFillColor(0);
ger50->Draw("same");
TGraph* ger10 = new TGraph(ner, ers1, er10);
ger10->SetLineStyle(2);
ger10->SetLineColor(kBlue-7);
ger10->SetLineWidth(width/1.5);
ger10->SetName("ger10");
ger10->Draw("same");
TGraph* ger90 = new TGraph(ner, ers1, er90);
ger90->SetLineStyle(2);
ger90->SetLineColor(kBlue-7);
ger90->SetLineWidth(width/1.5);
ger90->SetName("ger90");
ger90->Draw("same");
}

if (drawnr){
  iLine=-1;iEvent=0;
  int nnr=-1;
  double nrs1[500];
  double nr10[500];
  double nr50[500];
  double nr90[500];
  TString nrfile = TString::Format("/home/wxj/EFT_Bkg/DataValidation/NR_TB%d_DT%d.txt",timeBin, driftBin );
  cout<<nrfile.Data()<<endl;
  infile.open(nrfile.Data());
  if(!infile){
    cout<<"Invalid Input File!!!!!\n";
  }

  //skip first two lines
  //read dat file
  while(!infile.eof()){
    iLine++;
    nnr++; 
    //		if( iLine % 1000 == 0 ) {cout<<"Reading line "<<iLine<<endl;}
    std::getline(infile, readline);
    std::istringstream iss(readline);
    if(!(iss>>nrs1[nnr]>>nr10[nnr]>>nr50[nnr]>>nr90[nnr])){
      iLine++; cout<<"error/end: "<<endl; std::getline(infile, readline); cout<<readline<<endl; continue;}	
  }
  infile.close();

for (int ii=0; ii<nnr; ii++){
  nr10[ii]=log10(nr10[ii]);
  nr50[ii]=log10(nr50[ii]);
  nr90[ii]=log10(nr90[ii]);
//  cout<<nr50[ii]<<endl;
}
TGraph* gnr50 = new TGraph(nnr, nrs1, nr50);
gnr50->SetLineStyle(1);
gnr50->SetLineColor(kRed-7);
gnr50->SetLineWidth(width);
gnr50->SetName("gnr50");
gnr50->SetFillColor(0);
gnr50->Draw("same");
TGraph* gnr10 = new TGraph(nnr, nrs1, nr10);
gnr10->SetLineStyle(2);
gnr10->SetLineColor(kRed-7);
gnr10->SetLineWidth(width/1.5);
gnr10->SetName("gnr10");
gnr10->Draw("same");
TGraph* gnr90 = new TGraph(nnr, nrs1, nr90);
gnr90->SetLineStyle(2);
gnr90->SetLineColor(kRed-7);
gnr90->SetLineWidth(width/1.5);
gnr90->SetName("gnr90");
gnr90->Draw("same");
}


  if(drawolder||drawoldnr){
  int nolder=-1;
  int noldnr=-1;
  double erolds1[500];
  double eroldwid[500];
  double erold10[500];
  double erold50[500];
  double erold90[500];
  double nrolds1[500];
  double nroldwid[500];
  double nrold10[500];
  double nrold50[500];
  double nrold90[500];
  double NESTmean, NESTmeane, NESTmeanErr, NESTmeanErre,	NESTwidth, NESTwidthe,	NESTwidthErr,NESTwidthErr;
  TString ERoldfile = TString::Format("/home/wxj/EFT_Bkg/DataValidation/Run4Models_v3/TimeBin%d_%d_%d_ER_data.txt",timeBin, int(40.+(driftBin-1.)*65.), int(40.+(driftBin)*65.)  );
  cout<<ERoldfile.Data()<<endl;
  std::string line;
  infile.open(ERoldfile.Data());
  if(!infile){
    cout<<"Invalid Input File!!!!!\n";
  }

  //skip first two lines
//  std::string readline;	
  std::getline(infile, readline);
  std::getline(infile, readline);
  //read dat file
  while(!infile.eof()){
    iLine++;
    nolder++; 
    noldnr++; 
    //		if( iLine % 1000 == 0 ) {cout<<"Reading line "<<iLine<<endl;}
    std::getline(infile, readline);
    std::istringstream iss(readline);
    if(!(iss>>erolds1[nolder]>>erold50[nolder]>>NESTmeane>>eroldwid[nolder]>>NESTmeanErre>>nrold50[nolder]>> NESTwidthe>>nroldwid[nolder]>>NESTwidthErr)){
      iLine++; cout<<"error/end: "<<endl; std::getline(infile, readline); cout<<readline<<endl; continue;}	
  }
  infile.close();

for (int ii=0; ii<nolder; ii++){
  nrolds1[ii] = erolds1[ii];
  erold10[ii]=erold50[ii]-1.282*eroldwid[ii]+ log10(erolds1[ii]);
  erold90[ii]=erold50[ii]+1.282*eroldwid[ii]+ log10(erolds1[ii]);
  erold50[ii]=erold50[ii]+ log10(erolds1[ii]);
  nrold10[ii]=nrold50[ii]-1.282*nroldwid[ii]+ log10(nrolds1[ii]);
  nrold90[ii]=nrold50[ii]+1.282*nroldwid[ii]+ log10(nrolds1[ii]);
  nrold50[ii]=nrold50[ii]+ log10(nrolds1[ii]);
//  cout<<nrolds1[ii]<<"\t"<<nrold50[ii]<<"\t"<<nrold10[ii]<<"\t"<<nrold90[ii]<<endl;
}

if(drawolder){
TGraph* gerold50 = new TGraph(nolder-1, erolds1, erold50);
gerold50->SetLineStyle(1);
gerold50->SetLineColor(kBlue+2);
gerold50->SetLineWidth(width);
gerold50->SetName("gerold50");
gerold50->SetFillColor(0);
gerold50->Draw("same");
TGraph* gerold10 = new TGraph(nolder-1, erolds1, erold10);
gerold10->SetLineStyle(2);
gerold10->SetLineColor(kBlue+2);
gerold10->SetLineWidth(width/1.5);
gerold10->SetName("gerold10");
gerold10->Draw("same");
TGraph* gerold90 = new TGraph(nolder-1, erolds1, erold90);
gerold90->SetLineStyle(2);
gerold90->SetLineColor(kBlue+2);
gerold90->SetLineWidth(width/1.5);
gerold90->SetName("gerold90");
gerold90->Draw("same");
}


if(drawoldnr){
TGraph* gnrold50 = new TGraph(noldnr-1, nrolds1, nrold50);
gnrold50->SetLineStyle(1);
gnrold50->SetLineColor(kRed+2);
gnrold50->SetLineWidth(width);
gnrold50->SetName("gnrold50");
gnrold50->SetFillColor(0);
gnrold50->Draw("same");
TGraph* gnrold10 = new TGraph(noldnr-1, nrolds1, nrold10);
gnrold10->SetLineStyle(2);
gnrold10->SetLineColor(kRed+2);
gnrold10->SetLineWidth(width/1.5);
gnrold10->SetName("gnrold10");
gnrold10->Draw("same");
TGraph* gnrold90 = new TGraph(noldnr-1, nrolds1, nrold90);
gnrold90->SetLineStyle(2);
gnrold90->SetLineColor(kRed+2);
gnrold90->SetLineWidth(width/1.5);
gnrold90->SetName("gnrold90");
gnrold90->Draw("same");
}

}


//------------------
//------Legend
//------------------
  TLegend* tl = new TLegend(0.15,0.8,0.85,0.9);
  tl->SetNColumns(3);
       if(drawolder) tl->AddEntry(gerold50, "ER band (Run 4 WS)");
  if(drawoldnr) tl->AddEntry(gnrold50, "NR band (Run 4 WS)");
      tl->AddEntry(datapoints, "data");
 if(drawer) tl->AddEntry(ger50, "ER band (Run 4 EFT/IDM)");
  if(drawnr) tl->AddEntry(gnr50, "NR band (Run 4 EFT/IDM)");


  tl->SetTextSize(0.03);
  tl->SetFillColor(kWhite);
  tl->SetFillStyle(0);
  tl->SetLineColor(kBlack);
  tl->SetTextColor(kBlack);
  tl->Draw("same");


  TImage *img1 = TImage::Create();
  TString figname1;
  figname1 = TString::Format("dataS1Log10S2_tb%d_dt%d.png", timeBin, driftBin) ;
  c1->Update();  
  img1->FromPad(c1);
  img1->WriteImage(figname1.Data());
  std::cout << figname1.Data()<<std::endl;

_file0->Close();
return 1;

}

#endif
