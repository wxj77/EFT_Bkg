// how to run
//root -q -l -b "drawValidation.C(\"/home/wxj/EFT_Bkg/DataValidation/eft_bg_data_trial_05182018.root\",1, 1)"

#ifndef DRAWPROJECTION_C
#define DRAWPROJECTION_C 1


//unit per kg per day
double EXPUniform=1.;
double EXP8B=2.e-5; //from wei's simulation    //  .16/250.9;// From Jeremy, 06302016, S1 [0 50], Log10S2 [2 4], DriftTime [40, 300] 
double EXPAccidental=0.0039/100; //from wei's simulation //nExpectedAccidental = 0.0037788 * EXPOSURE; //this is Tomasz's estimate for S2raw > 200 phd. from ImportAccidentalModel.h
double EXPAr37=0.9e-3;//estimate in data Wei. We cannot really distinguish Ar37 or ER abnormally. so I choose this value  
double EXPRnKr=20.e-3*0.01*70*0.67+ 2.99e-3*0.2*70*0.67 +1.e-2; //from wei's simulation: 20e-3mBq/kg Rn222 + 2.99 e-3 mBq/kg/day (8.68e3 counts/3.36e4 kg day) Kr85
double EXPComptonBottom=1.3e-2;
double EXPComptonRest= 9.e-2;
double EXPKr83=2.e-3;
double EXPXe127;
double EXPneutron; 

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
#include "../Parameters.h"
#include "bkgClass.C"
//#include "../PDFs/paramBkg.C"
//class bkgComp;
//class bkgCompTB;
//class bkgCompFull;

using namespace std;


double tmass[4][5]={                                                // ----->drift bin 1, drift bin 2, drift bin 3, drift bin 4, total
{31.85 ,    28.24 ,    24.92 ,    20.36 ,    105.4 },              //|  time bin 1
{    33.10 ,    29.19 ,    24.92 ,    19.99 ,    107.2 },          //|  time bin 2
{    31.40 ,    26.97 ,    22.84 ,    17.97 ,    99.2 },           //|  time bin 3
{    32.01 ,    27.00 ,    22.57 ,    16.83 ,    98.4} };          //v  time bin 4

double texpo[5]= {46.766  ,  46.731 ,   91.552 ,   146.923  ,  331.972 }; // ----->drift bin 1, drift bin 2, drift bin 3, drift bin 4, total

bool drawer=1;
bool drawnr=1;
bool drawolder=1;
bool drawoldnr=1;
bool drawpart=1;

int drawProjection(TString datafile= "/home/wxj/EFT_Bkg/DataValidation/eft_bg_data_trial_05182018.root",int timeBin=1, int driftBin=1, TString histname="r"){

double width=3.;

TString histTitle;
int hnbins=100;
double hxmin, hxmax, hwid=1.;
if (histname=="r") {histTitle = "; r [cm]; event rate [cts kg^{-1} day^{-1}]"; hxmin=0; hxmax=FIDRADIUS; hnbins= NRBIN;}
if (histname=="phi") {histTitle = "; phi ; event rate [cts kg^{-1} day^{-1}]"; hxmin=PHIMIN; hxmax=PHIMAX; hnbins= NPHIBIN;}
if (histname=="drift") {histTitle = "; drift time [us]; event rate [cts kg^{-1} day^{-1}]";  hxmin=DTMIN; hxmax=DTMAX; hnbins= NDTBIN;} 
if (histname=="s1") {histTitle = "; s1c [phd]; event rate [cts kg^{-1} day^{-1}]";  hxmin=S1MIN; hxmax=S1MAX; hnbins= NS1BIN;}
if (histname=="log10s2") {histTitle = "; log_{10} s2c [phd]; event rate [cts kg^{-1} day^{-1}]";  hxmin=LOG10S2MIN; hxmax=LOG10S2MAX; hnbins= NLOGS2BIN;}
hwid = (hxmax-hxmin)/hnbins;
TH1F* h1 = new TH1F(histname.Data(), histTitle.Data(), hnbins, hxmin, hxmax);
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
  if (intb && indt) {h1->Fill(e.r);}  
}
h1->Scale(1./tmass[timeBin-1][driftBin-1]/texpo[timeBin-1]/hwid);

//----------
//load background components
bkgCompFull cr = bkgCompFull("ComptonRest");
bkgCompFull cb = bkgCompFull("ComptonBottom");
bkgCompFull rk = bkgCompFull("RnKr");


TH1F* rk_drift= (TH1F*) rk.bkg_tb[timeBin-1].bkg_dt[driftBin-1].rphidrift->Project3D("x");
cout<<rk.bkg_tb[timeBin-1].bkg_dt[driftBin-1].rphidrift->Integral();
rk_drift->SetDirectory(0);
rk_drift->Scale(tmass[timeBin-1][4]/tmass[timeBin-1][driftBin-1]);
TH1F* cr_drift= (TH1F*) cr.bkg_tb[timeBin-1].bkg_dt[driftBin-1].rphidrift->Project3D("x");
cr_drift->SetDirectory(0);
cr_drift->Scale(tmass[timeBin-1][4]/tmass[timeBin-1][driftBin-1]);
TH1F* cb_drift= (TH1F*) cb.bkg_tb[timeBin-1].bkg_dt[driftBin-1].rphidrift->Project3D("x");
cb_drift->SetDirectory(0);
cb_drift->Scale(tmass[timeBin-1][4]/tmass[timeBin-1][driftBin-1]);
TH1F* total_drift = (TH1F*) rk_drift->Clone("cb_total");
/*
for (int ii=1; ii<= rk_drift->GetNbinsX(); ii++) {
  cout<<rk_drift->GetBinContent(ii)/rk_drift->GetBinWidth(ii)<<endl;
  rk_drift->SetBinContent(ii, rk_drift->GetBinContent(ii)/rk_drift->GetBinWidth(ii));
  cout<<rk_drift->GetBinContent(ii)<<endl;
  cr_drift->SetBinContent(ii, cr_drift->GetBinContent(ii)/cr_drift->GetBinWidth(ii));
  cb_drift->SetBinContent(ii, cb_drift->GetBinContent(ii)/cb_drift->GetBinWidth(ii));
  }
  rk_drift->Draw();
  */
cout<<EXPRnKr/hwid<<endl;
rk_drift->Scale(EXPRnKr/hwid);
cr_drift->Scale(EXPComptonRest/hwid);
cb_drift->Scale(EXPComptonBottom/hwid);

int nn=rk_drift->GetNbinsX();
double total=0, value=0;
for (int ii=1; ii<= nn; ii++) {
  total_drift->SetBinContent(ii, 0);
  }
  total_drift->Add(rk_drift);
  total_drift->Add(cr_drift);
  total_drift->Add(cb_drift); 
  
//------------  
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
  lzStyle->SetLabelOffset(0.03, "y");
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
h1->SetLineColor(kBlack);
h1->SetLineWidth(width);
h1->GetYaxis()->SetRangeUser(0,0.008);
h1->GetYaxis()->SetRangeUser(0,0.05);
h1->GetYaxis()->SetTitleOffset(2.0);
h1->Draw();
rk_drift->SetLineColor(kGreen+2);
rk_drift->SetLineWidth(width/1.5);
rk_drift->Draw("same");

cb_drift->SetLineColor(kYellow+2);
cb_drift->SetLineWidth(width/1.5);
cb_drift->Draw("same");

cr_drift->SetLineColor(kRed+2);
cr_drift->SetLineWidth(width/1.5);
cr_drift->Draw("same");

total_drift->SetLineColor(kBlue);
total_drift->SetLineWidth(width);
total_drift->Draw("same");
//------------------
//------Legend
//------------------
  TLegend* tl = new TLegend(0.15,0.8,0.85,0.9);
  tl->SetNColumns(2);
      tl->AddEntry(h1, "data");

tl->AddEntry(total_drift, "simulation (total)");
if (drawpart){
tl->AddEntry(rk_drift, "simulation (Rn+Kr)");
tl->AddEntry(cb_drift, "simulation (Compton bottom detector)");
tl->AddEntry(cr_drift, "simulation (Compton other source)");
}
  tl->SetTextSize(0.03);
  tl->SetFillColor(kWhite);
  tl->SetFillStyle(0);
  tl->SetLineColor(kBlack);
  tl->SetTextColor(kBlack);
  tl->Draw("same");


  TImage *img1 = TImage::Create();
  TString figname1;
  figname1 = TString::Format("r_tb%d_dt%d.png", timeBin, driftBin) ;
  c1->Update();  
  img1->FromPad(c1);
  img1->WriteImage(figname1.Data());
  std::cout << figname1.Data()<<std::endl;

_file0->Close();
return 1;

}

#endif
