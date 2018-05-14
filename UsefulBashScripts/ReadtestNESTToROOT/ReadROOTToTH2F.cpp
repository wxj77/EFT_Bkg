// This codes is to read the root file from ReadtestNESTToROOT and format it to TH2F_S1_LOGS2
//WJ 2018.03.20
// how to run
//root -q -b -l "./ReadROOTToTH2F.cpp(\"result.root\", \"source\", TB)"


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
cout<<"success 1"<<endl;	
	TTree* in_tree = (TTree*)(in_file->Get("events"));	

cout<<"success 1"<<endl;
//	TTree* out_tree = new TTree("events", "LUX Fast Bkg Events Tree 8B");
		Float_t r, drift, phi, S1, log10S2, S2, rawS2, DT;
		in_tree->SetBranchAddress("DriftTime",&DT);
		in_tree->SetBranchAddress("s1c",&S1);
		in_tree->SetBranchAddress("s2c",&S2);
		in_tree->SetBranchAddress("log10S2",&log10S2);
		Int_t nentries = (Int_t)in_tree->GetEntries();
//create TH2F histograms
cout <<"you have " << nDtModel << " drift bins. "<<endl;
if (nDtModel!=4) {cout <<"you do not have 4 drift bins. "<<endl;}

double stepDT= (maxDt-minDt)/nDtModel;

//S1 log10S2
TString outputFile = TString::Format("../../PDFs/Bkg_S1Log10S2_TH2F_%s_TB%d.root",SourceName.Data(), TB);
TFile* out_file = new TFile( outputFile.Data(), "recreate");
out_file->cd();

int tb=1;
for( tb=1;tb<=nDtModel; tb++)
	{
		
		
//		TString histname = "B8";
        TString histname = TString::Format("DT%d", tb);
		TH2F* h1 = new TH2F(histname.Data(),"; s1_{c} [phd]; log_{10}s2_{c} [phd]",NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
	
	
		for(Int_t evt=0; evt<nentries; evt++){
			in_tree->GetEntry(evt);

			bool ins1 = (minS1<S1) && (S1 <=maxS1);
			bool ins2 = (minS2<S2) && (S2 <=maxS2);
			bool inlog10s2 = (minlog10S2<log10(S2)) && (log10(S2)<=maxlog10S2);
			bool indt = (minDt<DT) && (DT <=minDt+(tb)*stepDT);
			if (ins1 && inlog10s2 && indt) {h1->Fill(S1,log10(S2));}
		}
			out_file->cd();
		//h1->Scale(1./h1->Integral());
		h1->Write();
  		
	}
out_file->Close();

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
		//h1->Scale(1./h1->Integral());
		h1->Write();
  		
	}
out_file2->Close();


	return 1;

  	cout<<"Done"<<endl;
  	cout<<endl;
}










