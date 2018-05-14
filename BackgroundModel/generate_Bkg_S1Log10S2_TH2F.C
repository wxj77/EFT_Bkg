#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TRandom3.h"
#include <math.h> 
#include "TMath.h"
#include "../Parameters.h"
#include "modelBinDef.h"
#include "../EventEfficiency.h"
#include "../BandCuts.h"
#include "NEST.h"

using namespace std;

void generate_Bkg_S1Log10S2_TH2F(int numPts=1000, int timeBin=1) {

   // Open up the output file
   TFile f(TString::Format("./PDFs/Bkg_S1Log10S2_TH2F_TB%d.root",timeBin),"RECREATE");
   // Get NEST fired up
   int particleType = 1;
   double intialEnergy=1;
   double dt_init=300;
   double Xe_rho = 2.888;
   double df = 180.;
   NEST myNEST(particleType,intialEnergy,df,Xe_rho,dt_init);
   // Choose the detector parameters based on the time bin
   Detector myDetector;
   switch (timeBin) {
      case 1: myDetector.LUXRun04PlanBSettingsTime1();
	      break;
      case 2: myDetector.LUXRun04PlanBSettingsTime2();
	      break;
      case 3: myDetector.LUXRun04PlanBSettingsTime3();
	      break;
      case 4: myDetector.LUXRun04PlanBSettingsTime4();
	      break;
      default: cout << "COUT:: Invalid time bin (" << timeBin << ") passed to generate_Bkg_S1Log10S2_TH2F. Running libNEST with time bin 1 detector values." << endl;
	      myDetector.LUXRun04PlanBSettingsTime1();
   }
   myNEST.SetDetectorParameters(myDetector);
   cout << "COUT:: Loaded NEST with detector " << myDetector.name << endl;
   // Fill S1 vs. log10S2 hisograms (conditional on fields)
   // Get our model bin def, X = r, Y = phi, Z = drift:
   // Though this was coded for general voxelization, our current Plan B+ has model bins that only vary with drift time
   TH3F* h_binDef = modelBinDef(timeBin);
   TH3F* h_binFrDef = modelBinFrDef(timeBin);
   const int nModelBinsX = h_binDef->GetNbinsX();
   const int nModelBinsY = h_binDef->GetNbinsY();
   const int nModelBinsZ = h_binDef->GetNbinsZ();
   // Bounds of BG model uniform energy spectrum
   double Emin = 0; // keV
   double Emax = 20; // keV
   TRandom3 rando(0);
   // Loop over model bins. For each model bin, create new TH2F* and fill it with libNEST
   ofstream leakagefile;
   leakagefile.open(TString::Format("leakage_tb%d.txt",timeBin));
   
   int linearCounter = 0;
   double below_NR_mean_evts = 0.;
   double above_ER_mean_evts = 0.;
   double weight;
   for (int ix = 0; ix < nModelBinsX; ix++) {
      for (int iy = 0; iy < nModelBinsY; iy++) {
	 for (int iz = 0; iz < nModelBinsZ; iz++) {
	    // Debugging printout: make sure our indexing makes sense
	    int modelBin = ix*NPHIBINSMODEL*NDTBINSMODEL + iy*NDTBINSMODEL + iz;
	    //cout << "COUT (ix,iy,iz) = (" << ix << "," << iy<< "," << iz<< ")" << endl;
	    //cout << "COUT:: linearCounter = " << linearCounter << "\t modelBin = " << modelBin << "\tS1log10S2Hist_array.size()-1 = " << S1log10S2Hist_array.size()-1 << endl;
	    linearCounter++;
	    // Declare TH2
	    TString histname = TString::Format("h_S1vslog10S2_TimeBin%d__R%d_PHI%d_DT%d",timeBin,ix+1,iy+1,iz+1);
	    TH2F *h = new TH2F(histname.Data(),histname.Data(),NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
	    // Set the appropriate field
	    double Efield = h_binDef->GetBinContent( h_binDef->GetBin(ix+1,iy+1,iz+1) );
	    double Fr = h_binFrDef->GetBinContent( h_binFrDef->GetBin(ix+1,iy+1,iz+1) );
	    myNEST.SetElectricField(Efield);
	    myNEST.SetFr( Fr );
	    cout << "COUT:: About to generate ER band for model bin " << modelBin+1 << ", with E-field = " << Efield << " V/cm" << endl;
	    // Simulate numPts to fill the ER band for this model bin
	    for (int ii=0; ii<numPts; ii++) {
	       // Get random energy and drift (within model bin... this is slightly wrong, but it's how Jeremy does it).
	       double Ethis = Emin + (Emax - Emin)*rando.Rndm();
	       double dtRun3 = h_binDef->GetZaxis()->GetBinLowEdge(iz+1) + h_binDef->GetZaxis()->GetBinWidth(iz+1) * rando.Rndm(); 
	       //cout << "DEBUG:: running NEST with dt = " << dtRun3 << endl;
	       myNEST.SetEnergy(Ethis);
	       myNEST.SetDriftLocation(dtRun3);
	       myNEST.DetectorResponse();

	       double S2Raw = myNEST.GetS2();
	       double S2c = myNEST.GetS2c();
	       double S1Cor = myNEST.GetS1c();
	       if( S2Raw > S2RAWTHRESHOLD && log10(S2c)<ERCut_3sig(S1Cor) && log10(S2c)>NRCut_7sig(S1Cor) ){
		  h->Fill(S1Cor,log10(S2c),EventEfficiency(S2Raw));
	       }
               
               if( S2Raw>S2RAWTHRESHOLD && S1Cor>S1MIN && S1Cor<S1MAX && log10(S2c)>LOG10S2MIN && log10(S2c)<LOG10S2MAX 
                   && log10(S2c)<ERCut_3sig(S1Cor) && log10(S2c)>NRCut_7sig(S1Cor) ){ //selection cuts
		  
		  if( log10(S2c)>(3.04*pow(S1Cor,-0.104)+log10(S1Cor)) ){
		    above_ER_mean_evts += EventEfficiency(S2Raw); //above ER mean
		    //cout<<"Event above the mean with eff = "<<EventEfficiency(S2Raw)<<endl;
		  }
		  if( log10(S2c)<(2.54*pow(S1Cor,-0.09)+log10(S1Cor)) ){
		    below_NR_mean_evts += EventEfficiency(S2Raw); //below NR mean
		    //cout<<"Event below the NR mean with eff = "<<EventEfficiency(S2Raw)<<endl;
		  }
	       }
               
	       if ((ii+1) % 100000 == 0) cout << "COUT:: "<< ii+1  << " events generated in model bin " << modelBin+1 << endl;
	    } // Loop over events
	    h->Sumw2();
	    h->Smooth(NSMOOTH);
	    double integral =  h->Integral();
	    h->Scale(1./integral);
	    // Write to file
	    h->Write();
	    
	    cout<<endl;
	    cout<< above_ER_mean_evts << " events above the ER mean"<<endl;
	    cout<< below_NR_mean_evts << " events below the NR mean"<<endl;
	    cout<<"Time Bin "<<timeBin<<", Model Bin "<<modelBin+1<<" ratio (below NR mean/above ER mean) = "
	    << below_NR_mean_evts / above_ER_mean_evts <<endl;
	    
	    leakagefile<< below_NR_mean_evts / above_ER_mean_evts <<endl;
	    
	    cout<<endl;
	    
	 }// drift bins
      }//phi bins
   }// r bins
   leakagefile.close();
   f.Close();
}

void generate_Bkg_S1Log10S2_TH2F_Ar37(int numPts=1000, int timeBin=1) {

   // Open up the output file
   TFile f(TString::Format("./PDFs/Bkg_S1Log10S2_TH2F_Ar37_TB%d.root",timeBin),"RECREATE");
   // Get NEST fired up
   int particleType = 1;
   double intialEnergy=1;
   double dt_init=300;
   double Xe_rho = 2.888;
   double df = 180.;
   NEST myNEST(particleType,intialEnergy,df,Xe_rho,dt_init);
   // Choose the detector parameters based on the time bin
   Detector myDetector;
   switch (timeBin) {
      case 1: myDetector.LUXRun04PlanBSettingsTime1();
	      break;
      case 2: myDetector.LUXRun04PlanBSettingsTime2();
	      break;
      case 3: myDetector.LUXRun04PlanBSettingsTime3();
	      break;
      case 4: myDetector.LUXRun04PlanBSettingsTime4();
	      break;
      default: cout << "COUT:: Invalid time bin (" << timeBin << ") passed to generate_Bkg_S1Log10S2_TH2F. Running libNEST with time bin 1 detector values." << endl;
	      myDetector.LUXRun04PlanBSettingsTime1();
   }
   myNEST.SetDetectorParameters(myDetector);
   cout << "COUT:: Loaded NEST with detector " << myDetector.name << endl;
   // Fill S1 vs. log10S2 hisograms (conditional on fields)
   // Get our model bin def, X = r, Y = phi, Z = drift:
   // Though this was coded for general voxelization, our current Plan B+ has model bins that only vary with drift time
   TH3F* h_binDef = modelBinDef(timeBin);
   TH3F* h_binFrDef = modelBinFrDef(timeBin);
   const int nModelBinsX = h_binDef->GetNbinsX();
   const int nModelBinsY = h_binDef->GetNbinsY();
   const int nModelBinsZ = h_binDef->GetNbinsZ();
   // Bounds of BG model uniform energy spectrum
   //double Emin = 0; // keV
   //double Emax = 20; // keV
   double Ethis = 2.8; //keV, Ar37
   TRandom3 rando(0);
   // Loop over model bins. For each model bin, create new TH2F* and fill it with libNEST
   int linearCounter = 0;
   for (int ix = 0; ix < nModelBinsX; ix++) {
      for (int iy = 0; iy < nModelBinsY; iy++) {
	 for (int iz = 0; iz < nModelBinsZ; iz++) {
	    // Debugging printout: make sure our indexing makes sense
	    int modelBin = ix*NPHIBINSMODEL*NDTBINSMODEL + iy*NDTBINSMODEL + iz;
	    //cout << "COUT (ix,iy,iz) = (" << ix << "," << iy<< "," << iz<< ")" << endl;
	    //cout << "COUT:: linearCounter = " << linearCounter << "\t modelBin = " << modelBin << "\tS1log10S2Hist_array.size()-1 = " << S1log10S2Hist_array.size()-1 << endl;
	    linearCounter++;
	    // Declare TH2
	    TString histname = TString::Format("h_S1vslog10S2_TimeBin%d__R%d_PHI%d_DT%d",timeBin,ix+1,iy+1,iz+1);
	    TH2F *h = new TH2F(histname.Data(),histname.Data(),NS1BIN,S1MIN,S1MAX,NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
	    // Set the appropriate field
	    double Efield = h_binDef->GetBinContent( h_binDef->GetBin(ix+1,iy+1,iz+1) );
	    double Fr = h_binFrDef->GetBinContent( h_binFrDef->GetBin(ix+1,iy+1,iz+1) );
	    myNEST.SetElectricField(Efield);
	    myNEST.SetFr( Fr );
	    cout << "COUT:: About to generate ER band for model bin " << modelBin+1 << ", with E-field = " << Efield << " V/cm" << endl;
	    // Simulate numPts to fill the ER band for this model bin
	    for (int ii=0; ii<numPts; ii++) {
	       // Get random energy and drift (within model bin... this is slightly wrong, but it's how Jeremy does it).
	       //double Ethis = Emin + (Emax - Emin)*rando.Rndm();
	       double dtRun3 = h_binDef->GetZaxis()->GetBinLowEdge(iz+1) + h_binDef->GetZaxis()->GetBinWidth(iz+1) * rando.Rndm(); 
	       //cout << "DEBUG:: running NEST with dt = " << dtRun3 << endl;
	       myNEST.SetEnergy(Ethis);
	       myNEST.SetDriftLocation(dtRun3);
	       myNEST.DetectorResponse();

	       double S2Raw = myNEST.GetS2();
	       double S2c = myNEST.GetS2c();
	       double S1Cor = myNEST.GetS1c();
	       if( S2Raw > S2RAWTHRESHOLD && log10(S2c)<ERCut_3sig(S1Cor) && log10(S2c)>NRCut_7sig(S1Cor) ){
		  h->Fill(S1Cor,log10(S2c),EventEfficiency(S2Raw));
	       }

	       if ((ii+1) % 100000 == 0) cout << "COUT:: "<< ii+1  << " events generated in model bin " << modelBin+1 << endl;
	    } // Loop over events
	    h->Sumw2();
	    h->Smooth(NSMOOTH);
	    double integral =  h->Integral();
	    h->Scale(1./integral);
	    // Write to file
	    h->Write();
	 }// drift bins
      }//phi bins
   }// r bins
   f.Close();
}
