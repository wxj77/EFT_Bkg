#ifndef TEST
#define TEST
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TString.h"
#include "../Parameters.h"

using namespace std;

void moveOldCode_UniformSpatial(){

tb=4;
TString fname=TString::Format("Bkg_rphidrift_TH3F_Uniform_TB%d.root", tb);
TFile* file = new TFile(fname.Data(),"recreate");
file->cd();

TString nh1=TString::Format("RnKr_rphidrift_tb%d_R1_PHI1_DT1", tb);
TH3F* DT1 = (TH3F*) _file0->Get(nh1.Data());
TString nh2=TString::Format("RnKr_rphidrift_tb%d_R1_PHI1_DT2", tb);
TH3F* DT2 = (TH3F*) _file0->Get(nh2.Data());
TString nh3=TString::Format("RnKr_rphidrift_tb%d_R1_PHI1_DT3", tb);
TH3F* DT3 = (TH3F*) _file0->Get(nh3.Data());
TString nh4=TString::Format("RnKr_rphidrift_tb%d_R1_PHI1_DT4", tb);
TH3F* DT4 = (TH3F*) _file0->Get(nh4.Data());

DT1->SetName("DT1");
DT2->SetName("DT2");
DT3->SetName("DT3");
DT4->SetName("DT4");

DT1->Write();
DT2->Write();
DT3->Write();
DT4->Write();


file->Write();
file->Close();
}
	
void projectOldCode_Accidental(){//from Bkg_Accidentals_S1Log10S2_TH2F.root	

TFile* _file0 = new TFile("Bkg_Accidentals_S1Log10S2_TH2F.root", "read");
TH2F* Accidentals_S1vslog10S2 = (TH2F*) _file0->Get("Accidentals_S1vslog10S2");

TString fname=TString::Format("ParseS1S2.root");
TFile* file = new TFile(fname.Data(),"recreate");
file->cd();
TH1F* Accidental_S1 = new TH1F("Accidental_S1", "; S1 [phd]; event rate [cts day^{-1}]", 49, 1,50);
  for (int ii=1; ii<50; ii++){
    // cout<<ii<<endl;
     Accidental_S1->SetBinContent(ii, Accidentals_S1vslog10S2->GetBinContent(ii, 23));
  }

TH1F* Accidental_S2 = new TH1F("Accidental_S2", "; log_{10}S2 [phd]; event rate [cts day^{-1}]", 40, 2, 4);

TH1F* t3S2 = new TH1F("t3_S2", "; log_{10}S2 [phd]; event rate [cts day^{-1}]", 40, 2, 4);
TH1F* t40S2 = new TH1F("t40_S2", "; log_{10}S2 [phd]; event rate [cts day^{-1}]", 40, 2, 4);
 for (int ii=1; ii<=40; ii++){
    // cout<<ii<<endl;
     t3S2->SetBinContent(ii, Accidentals_S1vslog10S2->GetBinContent(3,ii));
     t40S2->SetBinContent(ii, Accidentals_S1vslog10S2->GetBinContent(40,ii));
  } 
  t3S2->Scale(1./(t3S2->GetBinContent(23)));
t40S2->Scale(1./(t40S2->GetBinContent(23)));
 for (int ii=1; ii<=40; ii++){
    // cout<<ii<<endl;
     if (ii<23) {Accidental_S2->SetBinContent(ii, t3S2->GetBinContent(ii));}
     else {Accidental_S2->SetBinContent(ii, t40S2->GetBinContent(ii));}
  }

Accidental_S1->Write();
Accidental_S2->Write();

file->Write();
file->Close();
}


Double_t fitf(Double_t *v, Double_t *par)
   {
      Double_t fitval = pow(10., par[0]+par[1]*log10(v[0]));
      return fitval;
   }
   void fitexample()
   {
      TFile *f = new TFile("hsimple.root");
   
      TH1F *hpx = (TH1F*)f->Get("hpx");

      //create a function with 3 parameters in the range [-3,3]
      TF1 *func = new TF1("fit",fitf,-3,3,3);
      func->SetParameters(500,hpx->GetMean(),hpx->GetRMS());
      func->SetParNames("Constant","Mean_value","Sigma");
      TF1 *func = new TF1("fit",fitf,5,50,2);
      Accidental_S1->Fit("fit","r");
      Accidental_S1->GetYaxis()->SetRangeUser(1.e-3,2.e-1);
      Accidental_S1->Draw();
TF1 *func2 = new TF1("fit",fitf,1,50,2);
      func2->SetParameters(func->GetParameter(0),func->GetParameter(1));
      func2->Draw("same");

   }

