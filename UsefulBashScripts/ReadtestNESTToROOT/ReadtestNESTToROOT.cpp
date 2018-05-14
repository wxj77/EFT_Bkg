// This codes is to read the txt file from testNEST and format it to ROOT
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
//#include "../modelBinDef.h"

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

using namespace std;

int main(int argc, char** argv){
	// arguments for main.
	if(argc != 3){
		cout<<"Invalid Input Parameters: ./ReadtestNESTToRoot testNEST.dat result.root\n./ReadtestNESTToRoot 8Bout.dat 8B.root ";
	 	return 0;
	}

//creat files.
	TString datafile= argv[1];
	TString outputFile= argv[2];
	TFile* out_file = new TFile( outputFile.Data(), "RECREATE");
	TTree* out_tree = new TTree("events", "LUX Fast Bkg Events Tree 8B");

//set branches
	int br_iEvtN = -1; out_tree->Branch("iEvtN", &br_iEvtN, "iEvtN/I");
//	int br_iParticleID = -1; out_tree->Branch("iParticleID", &br_iParticleID, "iParticleID/I");
	//real space positions
//	float br_x_cm = -999.; out_tree->Branch("x_cm", &br_x_cm, "x_cm/F");
//	float br_y_cm = -999.; out_tree->Branch("y_cm", &br_y_cm, "y_cm/F");
//	float br_r_cm = -999.; out_tree->Branch("r_cm", &br_r_cm, "r_cm/F");
	float br_z_cm = -999.; out_tree->Branch("z_cm", &br_z_cm, "z_cm/F");
//	float br_phi = -999.; out_tree->Branch("phi", &br_phi, "phi/F");
	//S2 space positions
//	float br_x_s2_cm = -999.; out_tree->Branch("x_s2_cm", &br_x_s2_cm, "x_s2_cm/F");
//	float br_y_s2_cm = -999.; out_tree->Branch("y_s2_cm", &br_y_s2_cm, "y_s2_cm/F");
//	float br_r_s2_cm = -999.; out_tree->Branch("r_s2_cm", &br_r_s2_cm, "r_s2_cm/F");
	float br_dt_s2 = -999.; out_tree->Branch("dt_s2", &br_dt_s2, "dt_s2/F");
//	float br_phi_s2 = -999.; out_tree->Branch("phi_s2", &br_phi_s2, "phi_s2/F");
	//Event energy
	float br_energy_keV = -999.; out_tree->Branch("energy_keV", &br_energy_keV, "energy_keV/F");
	//S1 info
	float br_s1_raw = -999.; out_tree->Branch("s1_raw", &br_s1_raw, "s1_raw/F");
	float br_s1c = -999.; out_tree->Branch("s1c", &br_s1c, "s1c/F");
	float br_s1s = -999.; out_tree->Branch("s1s", &br_s1s, "s1s/F");
	//S2 info
	float br_s2_raw = -999.; out_tree->Branch("s2_raw", &br_s2_raw, "s2_raw/F");
	float br_s2c = -999.; out_tree->Branch("s2c", &br_s2c, "s2c/F");
	float br_log10S2 = -999.; out_tree->Branch("log10S2", &br_log10S2, "log10S2/F");
	
//	float br_Fr = -999.; out_tree->Branch("Fr", &br_Fr, "Fr/F");
	float br_Efield = -999.; out_tree->Branch("Efield", &br_Efield, "Efield/F");
//	float br_vD = -999.; out_tree->Branch("vD", &br_vD, "vD/F");
	float br_DriftTime = -999.; out_tree->Branch("DriftTime", &br_DriftTime, "DriftTime/F");


	int iEvtN=0;
	int Nph, Ne, Nex = -1;
	
	int counter = 0;
	
	//Open up the .dat file
	//loop over events in the file and for each event get: S1c,S2c,r,z
	//save those in a .root file
	std::string line;
	ifstream infile(datafile.Data());
	long long iLine=-1,iEvent=0;
	if(!infile){
		cout<<"Invalid Input File!!!!!\n";
	}

//skip first two lines
	std::string readline;	
	std::getline(infile, readline);
	std::getline(infile, readline);

//read dat file
	while(!infile.eof()){
		iLine++;
		if( iLine % 1000 == 0 ) {cout<<"Reading line "<<iLine<<endl;}
		std::getline(infile, readline);
		std::istringstream iss(readline);
		if(!(iss>>br_energy_keV>>br_Efield>>br_DriftTime>>br_z_cm>>Nph>>Ne
				>>br_s1_raw>>br_s1c>>br_s1s>>Nex>>br_s2_raw>>br_s2c)){
			iLine++; cout<<"error: "<<endl; std::getline(infile, readline); /*cout<<readline<<endl*/; continue;}	
		if (br_s1c<=0.){continue;}
		if (br_s2c<=0.){continue;}
		if (br_DriftTime > maxDt){continue;}
		if (br_DriftTime < minDt){continue;}
		br_z_cm = br_z_cm/10.;//convert mm to cm from br_z_cm.
		br_log10S2 = log10( br_s2c ); 
		out_tree->Fill(); //Fill new tree with info if single scatter
		counter++;iEvtN++;
	}
	infile.close();
	
	
	out_file->cd();
	out_tree->Write();
	out_file->Close();

//Finish
	cout<< counter << " events saved"<<endl;
	cout<<"Finished!"<<endl;
	return 1;
}


