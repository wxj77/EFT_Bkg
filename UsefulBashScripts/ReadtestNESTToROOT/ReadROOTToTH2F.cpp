// This codes is to read the root file from ReadtestNESTToROOT and format it to TH2F_S1_LOGS2
//WJ 2018.03.20
// how to run
//root -q -b -l "./ReadROOTToTH2F.cpp(\"result.root\", \"source\", TB)"

double maxentries=500000000000;

//C includes
#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <math.h>

//ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"

//other custom functions.
#include "../../Parameters.h"
#include "../EventCuts.C"
#include "../EventEfficiency.C"

#ifndef events_cxx
#include "events.C"
#endif

//read Parameters variables.
const int nBinsDt=NDTBIN;
const double minDt=DTMIN;
const double maxDt=DTMAX;

const int nDtModel=NDTBINSMODEL;

const int nBinsRadius=NRBIN;
const double maxRadius=FIDRADIUS;

const int nBinsPhi=NPHIBIN;
const double minPhi=PHIMIN;
const double maxPhi=PHIMAX;


const int nBinsS1=NS1BIN;
const double minS1=S1MIN;
const double maxS1=S1MAX;

const int nBinslog10S2=NLOGS2BIN;
const double minlog10S2 = LOG10S2MIN;
const double maxlog10S2 = LOG10S2MAX;

const int nBinsS2=NLS2BIN;
const double minS2 = LS2MIN;
const double maxS2 = LS2MAX;


const double minRawS2_phe=S2RAWTHRESHOLD;

const int livedays=EXPOSURE;


//int binMultiplier = 2;

using namespace std;

int ReadROOTToTH2F(TString datafile, TString SourceName, int TB){

  //create files



  cout<<datafile.Data()<<endl;
  TFile* in_file = new TFile(datafile.Data(), "READ");
  TTree* in_tree = (TTree*)(in_file->Get("events"));	



  //	TTree* out_tree = new TTree("events", "LUX Fast Bkg Events Tree 8B");
  events e(in_tree);
  in_tree->GetEntry(1);
  cout<< e.DriftTime<<endl;
  /*		Float_t r, drift, phi, S1, log10S2, S2, rawS2, DT;
		in_tree->SetBranchAddress("DriftTime",&DT);
		in_tree->SetBranchAddress("s1c",&S1);
		in_tree->SetBranchAddress("s2c",&S2);
		in_tree->SetBranchAddress("log10S2",&log10S2);*/
		double nentries = (double)in_tree->GetEntries();
  if (nentries > maxentries) {nentries=maxentries;}
  cout <<"you have " << nentries << " events to run. "<<endl;
  //create TH2F histograms
  cout <<"you have " << nDtModel << " drift bins. "<<endl;
  if (nDtModel!=4) {cout <<"you do not have 4 drift bins. "<<endl;}

  double stepDT= (maxDt-minDt)/nDtModel;

cout <<"min, max drift time:"<<minDt<<"\t"<<minDt+(4.)*stepDT<<endl;

  int tb=1;
  double totalpasscut;  
  double totalpasscut_tb[5];
  double integpasscut;
  double integpasscut_tb[5];



//--------------------------------------------------------------
// Fill S1Log10S2 tree
  //S1 log10S2
  TString outputFile = TString::Format("../../PDFs/Bkg_S1Log10S2_TH2F_%s_TB%d.root",SourceName.Data(), TB);
  TFile* out_file = new TFile( outputFile.Data(), "recreate");

  out_file->cd();

  totalpasscut=0;
  integpasscut=0;
  for (int ii=0; ii<5; ii++){
    totalpasscut_tb[ii]=0;
    integpasscut_tb[ii]=0;
  }

  if(1){
		
		
      //		TString histname = "B8";
      TString histname = TString::Format("DT%d", tb);
   /*   TH2F* h1 = new TH2F(histname.Data(),"; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
      histname = TString::Format("DT%d_nocut", tb);
      TH2F* h2 = new TH2F(histname.Data(),"; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
  */    
      TH2F* hdt1 = new TH2F("DT1","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
      TH2F* hdt1_nocut = new TH2F("DT1_nocut","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);	
	  TH2F* hdt2 = new TH2F("DT2","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
      TH2F* hdt2_nocut = new TH2F("DT2_nocut","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);	
      TH2F* hdt3 = new TH2F("DT3","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
      TH2F* hdt3_nocut = new TH2F("DT3_nocut","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);	
      TH2F* hdt4 = new TH2F("DT4","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
      TH2F* hdt4_nocut = new TH2F("DT4_nocut","; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);	
	
      for(Int_t evt=0; evt<nentries; evt++){
	in_tree->GetEntry(evt);
	if(evt % 1000 == 0 ) {cout<<"Reading line "<<evt<<endl;}

	bool ins1 = (minS1<e.s1c) && (e.s1c <=maxS1);
	bool ins2 = (minS2<e.s2c) && (e.s2c <=maxS2);
	bool inlog10s2 = (minlog10S2<log10(e.s2c)) && (log10(e.s2c)<=maxlog10S2);
	bool indt = (minDt+(1.-1)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(4.)*stepDT);
	
	bool inrange= ins1 && (inlog10s2) && indt;
    if (!inrange) {continue;}	
	
	bool indt1 = (minDt+(1.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(1.)*stepDT);
	bool indt2 = (minDt+(2.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(2.)*stepDT);
	bool indt3 = (minDt+(3.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(3.)*stepDT);
	bool indt4 = (minDt+(4.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(4.)*stepDT);

	double e_light[5] = {e.s1_raw, e.s1c, e.s1s, e.s2_raw, e.s2c};
	bool incut = EventCuts(e_light);
	double cuteff = EventEfficiency(e_light);
    if (inrange && indt1  ) {hdt1_nocut->Fill(e.s1c,log10(e.s2c));}
	if (inrange && indt1 && incut) {hdt1->Fill(e.s1c,log10(e.s2c), cuteff);totalpasscut_tb[1]+=1;}
	if (inrange && indt2  ) {hdt2_nocut->Fill(e.s1c,log10(e.s2c));}
	if (inrange && indt2 && incut) {hdt2->Fill(e.s1c,log10(e.s2c), cuteff);totalpasscut_tb[2]+=1;}
	if (inrange && indt3  ) {hdt3_nocut->Fill(e.s1c,log10(e.s2c));}
	if (inrange && indt3 && incut) {hdt3->Fill(e.s1c,log10(e.s2c), cuteff);totalpasscut_tb[3]+=1;}
	if (inrange && indt4  ) {hdt4_nocut->Fill(e.s1c,log10(e.s2c));}
	if (inrange && indt4 && incut) {hdt4->Fill(e.s1c,log10(e.s2c), cuteff);totalpasscut_tb[4]+=1;}
      }
      out_file->cd();
      integpasscut_tb[1]+=hdt1->Integral();
      integpasscut_tb[2]+=hdt2->Integral();
      integpasscut_tb[3]+=hdt3->Integral();
      integpasscut_tb[4]+=hdt4->Integral();
      integpasscut=0;
      for (int tb=1; tb<= nDtModel; tb++){
        integpasscut+=integpasscut_tb[tb];
        totalpasscut+=totalpasscut_tb[tb];
      }
      
      
 //    h1->Scale(1./h1->Integral()); //scale pdf to 1.
 //     h1->Write();
 //     h2->Write();
  		
  }   
  TTree* header= new TTree("header", "stats from simulation.");
  header->Branch("totalpasscut", &totalpasscut, "totalpasscut/D");
  header->Branch("totalpasscut_tb", &totalpasscut_tb, "totalpasscut_tb[5]/D");
  header->Branch("integpasscut", &integpasscut, "integpasscut/D");
  header->Branch("integpasscut_tb", &integpasscut_tb, "integpasscut_tb[5]/D");
  header->Branch("nentries", &nentries, "nentries/D");
  header->Fill();

  cout << "nunmber of events pass cut for S1Log10S2 in each time bin are:\n"<<"1: "<<totalpasscut_tb[1]<<"\t"<<"2: "<<totalpasscut_tb[2]<<"\t"<<"3: "<<totalpasscut_tb[3]<<"\t"<<"4: "<<totalpasscut_tb[4]<<"\t"<<endl; 	
  cout << "total fraction(not 8B) pass cut for S1Log10S2 is : "<< integpasscut/nentries << ""<<endl;
  cout << "total fraction(8B) pass cut for S1Log10S2 is : "<< integpasscut << "/(how many events you generated, typically 10000000.) cts kg^{-1} day^{-1}"<<endl;
  cout << "total fraction(8B) recorded(do not use this ) : "<< nentries <<endl;
  cout<<"Done"<<endl;
  cout<<endl;
  out_file->cd();
  header->Write();
  out_file->Write();
  delete header;
  out_file->Close();
  cout << "output file name: "<<outputFile.Data()<<endl;

//--------------------------------------------------------------
// Fill S1S2 tree
/*
  totalpasscut=0.;
  //S1S2
  TString outputFile2 = TString::Format("../../PDFs/Bkg_S1S2_TH2F_%s_TB%d.root",SourceName.Data(), TB);
  TFile* out_file2 = new TFile( outputFile2.Data(), "RECREATE");
  out_file2->cd();

  for(int tb=1; tb<=nDtModel; tb++)
    {
		
		
      //		TString histname = "B8";
      TString histname = TString::Format("DT%d", tb);
      TH2F* h1 = new TH2F(histname.Data(),"; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);

      for(Int_t evt=0; evt<nentries; evt++){
	in_tree->GetEntry(evt);

	bool ins1 = (minS1<S1) && (S1 <=maxS1);
	bool ins2 = (minS2<S2) && (S2 <=maxS2);
	bool inlog10s2 = (minlog10S2<log10(S2)) && (log10(S2)<=maxlog10S2);
	bool indt = (minDt<DT) && (DT <=minDt+(tb)*stepDT);
	if (ins1 && ins2 && indt) {h1->Fill(S1,S2);}
      }
      out_file2->cd();
      totalpasscut+=h1->Integral();
      h1->Scale(1./h1->Integral());
      h1->Write();
  		
    }





  cout << "total fraction(not 8B) pass cut for S1S2 is : "<< totalpasscut/in_tree->GetEntries() << ""<<endl;
  cout << "total fraction(8B) pass cut for S1S2 is : "<< totalpasscut << "/(how many events you generated, typically 10000000.) cts kg^{-1} day^{-1}"<<endl;
  cout << "total fraction(8B) recorded(do not use this ) : "<< in_tree->GetEntries() <<endl;
  
  out_file2->Close();
 



*/


  //S1S2
  TString outputFile2 = TString::Format("../../PDFs/Bkg_S1S2_TH2F_%s_TB%d.root",SourceName.Data(), TB);
  TFile* out_file2 = new TFile( outputFile2.Data(), "recreate");

  out_file2->cd();

  totalpasscut=0;
  integpasscut=0;
  for (int ii=0; ii<5; ii++){
    totalpasscut_tb[ii]=0;
    integpasscut_tb[ii]=0;
  }

  if(1){
		
		
      //		TString histname = "B8";
      TString histname = TString::Format("DT%d", tb);
   /*   TH2F* h1 = new TH2F(histname.Data(),"; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);
      histname = TString::Format("DT%d_nocut", tb);
      TH2F* h2 = new TH2F(histname.Data(),"; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);
  */    
      TH2F* hdt1 = new TH2F("DT1","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);
      TH2F* hdt1_nocut = new TH2F("DT1_nocut","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);	
	  TH2F* hdt2 = new TH2F("DT2","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);
      TH2F* hdt2_nocut = new TH2F("DT2_nocut","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);	
      TH2F* hdt3 = new TH2F("DT3","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);
      TH2F* hdt3_nocut = new TH2F("DT3_nocut","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);	
      TH2F* hdt4 = new TH2F("DT4","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);
      TH2F* hdt4_nocut = new TH2F("DT4_nocut","; s1_{c} [phd]; s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLS2BIN,LS2MIN,LS2MAX);	
	
      for(Int_t evt=0; evt<nentries; evt++){
	in_tree->GetEntry(evt);
	if(evt % 1000 == 0 ) {cout<<"Reading line "<<evt<<endl;}

	bool ins1 = (minS1<e.s1c) && (e.s1c <=maxS1);
	bool ins2 = (minS2<e.s2c) && (e.s2c <=maxS2);
	bool inlog10s2 = (minlog10S2<log10(e.s2c)) && (log10(e.s2c)<=maxlog10S2);
	bool indt = (minDt+(1.-1)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(4.)*stepDT);
	
	bool inrange= ins1 && (ins2) && indt;
    if (!inrange) {continue;}
    
	bool indt1 = (minDt+(1.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(1.)*stepDT);
	bool indt2 = (minDt+(2.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(2.)*stepDT);
	bool indt3 = (minDt+(3.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(3.)*stepDT);
	bool indt4 = (minDt+(4.-1.)*stepDT<e.DriftTime) && (e.DriftTime <=minDt+(4.)*stepDT);



	double e_light[5] = {e.s1_raw, e.s1c, e.s1s, e.s2_raw, e.s2c};
	bool incut = EventCuts(e_light);
	double cuteff = EventEfficiency(e_light);
    if (inrange && indt1  ) {hdt1_nocut->Fill(e.s1c,e.s2c);}
	if (inrange && indt1 && incut) {hdt1->Fill(e.s1c,e.s2c, cuteff);totalpasscut_tb[1]+=1;}
	if (inrange && indt2  ) {hdt2_nocut->Fill(e.s1c,e.s2c);}
	if (inrange && indt2 && incut) {hdt2->Fill(e.s1c,e.s2c, cuteff);totalpasscut_tb[2]+=1;}
	if (inrange && indt3  ) {hdt3_nocut->Fill(e.s1c,e.s2c);}
	if (inrange && indt3 && incut) {hdt3->Fill(e.s1c,e.s2c, cuteff);totalpasscut_tb[3]+=1;}
	if (inrange && indt4  ) {hdt4_nocut->Fill(e.s1c,e.s2c);}
	if (inrange && indt4 && incut) {hdt4->Fill(e.s1c,e.s2c, cuteff);totalpasscut_tb[4]+=1;}
      }
      out_file->cd();
      integpasscut_tb[1]+=hdt1->Integral();
      integpasscut_tb[2]+=hdt2->Integral();
      integpasscut_tb[3]+=hdt3->Integral();
      integpasscut_tb[4]+=hdt4->Integral();
      integpasscut=0;
      for (int tb=1; tb<= nDtModel; tb++){
        integpasscut+=integpasscut_tb[tb];
        totalpasscut+=totalpasscut_tb[tb];
      }
      
      
 //    h1->Scale(1./h1->Integral()); //scale pdf to 1.
 //     h1->Write();
 //     h2->Write();
  		
   }
  TTree* header2= new TTree("header", "stats from simulation.");
  header2->Branch("totalpasscut", &totalpasscut, "totalpasscut/D");
  header2->Branch("totalpasscut_tb", &totalpasscut_tb, "totalpasscut_tb[5]/D");
  header2->Branch("integpasscut", &integpasscut, "integpasscut/D");
  header2->Branch("integpasscut_tb", &integpasscut_tb, "integpasscut_tb[5]/D");
  header2->Branch("nentries", &nentries, "nentries/D");
  header2->Fill();
  
  cout << "nunmber of events pass cut for S1S2 in each time bin are:\n"<<"1: "<<totalpasscut_tb[1]<<"\t"<<"2: "<<totalpasscut_tb[2]<<"\t"<<"3: "<<totalpasscut_tb[3]<<"\t"<<"4: "<<totalpasscut_tb[4]<<"\t"<<endl; 	
  cout << "total fraction(not 8B) pass cut for S1S2 is : "<< integpasscut/nentries << ""<<endl;
  cout << "total fraction(8B) pass cut for S1S2 is : "<< integpasscut << "/(how many events you generated, typically 10000000.) cts kg^{-1} day^{-1}"<<endl;
  cout << "total fraction(8B) recorded(do not use this ) : "<< nentries <<endl;
  cout<<"Done"<<endl;
  cout<<endl;
  out_file2->cd();
  header2->Write();
  out_file2->Write();
  delete header2;
  out_file2->Close();
  cout << "output file name: "<<outputFile2.Data()<<endl;

















 
  cout<<"Done"<<endl;
  cout<<endl;
  	
  return 1;
}










