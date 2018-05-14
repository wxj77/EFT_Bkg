// This codes is to read the root file from ReadtestNESTToROOT and format it to TH2F_S1_LOGS2
//WJ 2018.03.20

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
#include "TString.h"

//other custom functions.
#include "../../Parameters.h"
#include "../modelBinDef.h"

//read Parameters variables.
const double minDt=DTMIN;
const double maxDt=DTMAX;
const double maxRadius=FIDRADIUS;
const double minPhi=PHIMIN;
const double maxPhi=PHIMAX;
const double minRawS2_phe=S2RAWTHRESHOLD;
const double minS1=S1MIN;
const double maxS1=S1MAX;
const double minlog10S2 = LOG10S2MIN;
const double maxlog10S2 = LOG10S2MAX;
const int nBinsS1=NS1BIN;
const int nBinslog10S2=NLOGS2BIN;
const int nBinsDt=NDTBIN;
const int nBinsRadius=NRBIN;
const int nBinsPhi=NPHIBIN;
const int livedays=EXPOSURE;

int binMultiplier = 2;

using namespace std;

int main(int argc, char** argv){
	// arguments for main.
	if(argc != 4){
		cout<<"Invalid Input Parameters: ./ReadROOTToTH2F result.root source\n./ReadtestNESTToRoot 8B.root 8B";
	 	return 0;
	}

//create files
	TString datafile= argv[1];
	TString SourceName= argv[2];
	int TB = atoi(argv[3]);

	TFile* in_file = TFile::Open(datafile.Data(), "READ");
	TTree* in_tree = (TTree* )(in_file->Get("events"));	

//	TTree* out_tree = new TTree("events", "LUX Fast Bkg Events Tree 8B");

//create TH2F histograms
for(int tb=TB; tb<=TB; tb++)
	{
		TString outputFile = TString::Format("../PDFs/Bkg_S1Log10S2_TH2F_8B_TB%d.root", tb);
		TFile* out_file = new TFile( outputFile.Data(), "RECREATE");
		out_file->cd();
		TString histname = "B8";
		TH2F* h_S1vslog10S2 = new TH2F(histname.Data(),histname.Data(),binMultiplier*NS1BIN,S1MIN,S1MAX,binMultiplier*NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
		Float_t r, drift, phi, S1, log10S2, rawS2;
		in_tree->SetBranchAddress("s1c",&S1);
		in_tree->SetBranchAddress("log10S2",&log10S2);	
		Int_t nentries = (Int_t)in_tree->GetEntries();	
		for(Int_t evt=0; evt<nentries; evt++){
			in_tree->GetEntry(evt);
			h_S1vslog10S2->Fill( S1, log10S2);
		}
			out_file->cd();
		h_S1vslog10S2->Write();
  		out_file->Close();
	}
	return 1;

  	cout<<"Done"<<endl;
  	cout<<endl;
}










