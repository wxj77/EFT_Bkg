#ifndef GENERATEAccidental
#define GENERAREAccidental 1

#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TFile.h"
#include "TString.h"
#include "../Parameters.h"


#include "IsoS1S2Spectrum.C"
//int GenerateAccidental(int tb=1);

int GenerateAccidental(TString folder="../PDFs/", int tb=1){

TF1* fisoS1 = new TF1("fisoS1",isoS1,0,1.e3,0);
TF1* fisoS2 = new TF1("fisoS2",isoS2,0,1.e6,0);

TH1F* Accidental_S1 = new TH1F("Accidental_S1", "; S1 [phd]; event rate [Hz]",NS1BIN ,S1MIN,S1MAX);
  for (int ii=1; ii<NS1BIN+1 ; ii++){
  
  
 /*   double x[1]={Accidental_S1->GetBinCenter(ii)};
     Accidental_S1->SetBinContent(ii, isoS1(x)*(Accidental_S1->GetBinLowEdge(ii+1)-  Accidental_S1->GetBinLowEdge(ii) ));
  */
  double xmin=Accidental_S1->GetBinLowEdge(ii);
  double xmax=Accidental_S1->GetBinLowEdge(ii+1);
  double value = fisoS1->Integral(xmin, xmax);
  Accidental_S1->SetBinContent(ii, value);
 // cout<<isoS1(x, NULL)*(Accidental_S1->GetBinLowEdge(ii+1)-  Accidental_S1->GetBinLowEdge(ii) )<<endl;
 // cout<<value<<endl;
  }

TH1F* Accidental_Log10S2 = new TH1F("Accidental_Log10S2", "; log_{10}S2 [phd]; event rate [Hz]", NLOGS2BIN, LOG10S2MIN, LOG10S2MAX);  
  for (int ii=1; ii<NLOGS2BIN+1 ; ii++){
  /*
    double x=pow(10.,Accidental_Log10S2->GetBinCenter(ii));
     Accidental_Log10S2->SetBinContent(ii, isoS2(x)*(pow(10,Accidental_Log10S2->GetBinLowEdge(ii+1))- pow(10., Accidental_Log10S2->GetBinLowEdge(ii)) ));
  */
  double xmin=pow(10., Accidental_Log10S2->GetBinLowEdge(ii));
  double xmax=pow(10., Accidental_Log10S2->GetBinLowEdge(ii+1));
  double value = fisoS2->Integral(xmin, xmax);
  Accidental_Log10S2->SetBinContent(ii, value);
  
  }

TH1F* Accidental_S2 = new TH1F("Accidental_S2", "; log_{10}S2 [phd]; event rate [Hz]", NLS2BIN, LS2MIN, LS2MAX);
  for (int ii=1; ii<NLS2BIN+1 ; ii++){
 /*
    double x=Accidental_S2->GetBinCenter(ii);
     Accidental_S2->SetBinContent(ii, isoS2(x)*(Accidental_S2->GetBinLowEdge(ii+1)-  Accidental_S2->GetBinLowEdge(ii) ));
  */
    double xmin=Accidental_S2->GetBinLowEdge(ii);
  double xmax=Accidental_S2->GetBinLowEdge(ii+1);
  double value = fisoS2->Integral(xmin, xmax);
  Accidental_S2->SetBinContent(ii, value);
  }


if(1){
TString fname=TString::Format("%s/Bkg_S1Log10S2_TH2F_Accidental_TB%d.root", folder.Data(),tb);
TFile* file = new TFile(fname.Data(),"recreate");
file->cd();
double scale = (DTMAX-DTMIN)/4. *1.e-6*24*60*60; //4 time bin, 1.e-6 us to s 24*60*60 s-1 to day^{-1} 

TH2F* DT1 = new TH2F("DT1", "; S1 [phd] ; S2 [phd]; event rate [cts day^{-1}]", NS1BIN ,S1MIN,S1MAX, NLOGS2BIN, LOG10S2MIN, LOG10S2MAX);
  for (int ii=1; ii<NS1BIN+1 ; ii++){
  for (int jj=1; jj<NLOGS2BIN+1 ; jj++){

    double x  =  Accidental_S1->GetBinContent(ii);
    double y =  Accidental_Log10S2->GetBinContent(jj);
//    if (ii==1)   cout<<x*y*scale<<endl;
     DT1->SetBinContent(ii, jj, x*y*scale);
  }

  }

TH2F* DT2 = (TH2F*)DT1->Clone();
TH2F* DT3 = (TH2F*)DT1->Clone();
TH2F* DT4 = (TH2F*)DT1->Clone();
  
Accidental_S1->Write();
Accidental_S2->Write();
Accidental_Log10S2->Write();

DT1->SetName("DT1");
DT2->SetName("DT2");
DT3->SetName("DT3");
DT4->SetName("DT4");

cout << "total counts expected for accidental model S1Log10S2 is : "<< DT1->Integral()+DT2->Integral()+DT3->Integral()+DT4->Integral() << " cts day^{-1}"<<endl;

DT1->Scale(1./DT1->Integral());
DT2->Scale(1./DT2->Integral());
DT3->Scale(1./DT3->Integral());
DT4->Scale(1./DT4->Integral());

DT1->Write();
DT2->Write();
DT3->Write();
DT4->Write();
file->Write();
file->Close();
}


if(1){
TString fname=TString::Format("%s/Bkg_S1S2_TH2F_Accidental_TB%d.root",folder.Data(), tb);
TFile* file = new TFile(fname.Data(),"recreate");
file->cd();
double scale = (DTMAX-DTMIN)/4. *1.e-6*24*60*60; //4 time bin, 1.e-6 us to s 24*60*60 s-1 to day^{-1} 

TH2F* DT1 = new TH2F("DT1", "; S1 [phd] ; S2 [phd]; event rate [cts day^{-1}]", NS1BIN ,S1MIN,S1MAX, NLS2BIN, LS2MIN, LS2MAX);
  for (int ii=1; ii<NS1BIN+1 ; ii++){
  for (int jj=1; jj<NLS2BIN+1 ; jj++){
    double x  =  Accidental_S1->GetBinContent(ii);
    double y =  Accidental_S2->GetBinContent(jj);
     DT1->SetBinContent(ii, jj, x*y*scale);
  }
  }

TH2F* DT2 = (TH2F*)DT1->Clone();
TH2F* DT3 = (TH2F*)DT1->Clone();
TH2F* DT4 = (TH2F*)DT1->Clone();
  
Accidental_S1->Write();
Accidental_S2->Write();
Accidental_Log10S2->Write();




DT1->SetName("DT1");
DT2->SetName("DT2");
DT3->SetName("DT3");
DT4->SetName("DT4");

cout << "total counts expected for accidental model S1S2 is : "<< DT1->Integral()+DT2->Integral()+DT3->Integral()+DT4->Integral() << " cts day^{-1}"<<endl;

DT1->Scale(1./DT1->Integral());
DT2->Scale(1./DT2->Integral());
DT3->Scale(1./DT3->Integral());
DT4->Scale(1./DT4->Integral());

DT1->Write();
DT2->Write();
DT3->Write();
DT4->Write();
file->Write();
file->Close();
}

return 1;
}
#endif
