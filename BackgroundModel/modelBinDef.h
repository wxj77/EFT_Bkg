#ifndef MODELBINDEF_H
#define MODELBINDEF_H
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TString.h"
#include "../Parameters.h"

using namespace std;

TH3F* modelBinDef(int timeBin = 1, TString histname = "h_binDef") {
// Define the edges of the Plan B+ "model bins" (or "voxels") with a TH3.
// Bin contents encode the electric field for each model bin to be passed
// to libNEST (as a function of input time bin), as well as errors.

   // Define r and phi bins. As of now, only 1 bin in these dimensions (only varying the model in drift)
   // (The factor of 1.01 is to avoid problems with the last bin edge)
   float rBins[NRBINSMODEL+1];
   for (int ii=0; ii<=NRBINSMODEL; ii++) {
      rBins[ii] = (FIDRADIUS*1.01 - 0)*(double)ii / (NRBINSMODEL) + 0;
   }
   float phiBins[NPHIBINSMODEL+1];
   for (int ii=0; ii<=NPHIBINSMODEL; ii++) {
      phiBins[ii] = (PHIMAX*1.01 - PHIMIN)*(double)ii / (NPHIBINSMODEL) + PHIMIN;
   }
   
   // From Jeremy's Plan B+ work:
   // http://teacher.pas.rochester.edu:8080/wiki/pub/Lux/AnalysisMeeting160412/20160411MockLibNEST_ER_Band.pptx.pdf
   float dtBins[NDTBINSMODEL+1] = {DTMIN , 105. , 170. , 235. , DTMAX*1.01}; // changed outer bin edges to match Parameters.h

   // THIS MUST BE UPDATED!!!!
   // Electric fields (V/cm) to pass to libNEST for each time bin and model bin
   // These come from readme.txt inside the libNEST repo
   float Efields[NTIMEBIN][NDTBINSMODEL] = { {322., 233., 170., 121.},    // time bin 1
					     {347., 207., 124., 78.},    // time bin 2
					     {332., 256., 152., 97.},    // time bin 3
					     {382., 286., 156., 86.} };  // time bin 4
					     
   float Efields_error[NTIMEBIN][NDTBINSMODEL] = { {37., 28., 21., 14.},    // time bin 1
					           {62., 44., 26., 14.},    // time bin 2
					           {17., 14., 9., 6.},    // time bin 3
					           {47., 34., 19., 12.} };  // time bin 4
   // Declare the histogram which defines the voxelization of the detector.
   TH3F* h_binDef = new TH3F(histname.Data(),histname.Data(),NRBINSMODEL,rBins,NPHIBINSMODEL,phiBins,NDTBINSMODEL,dtBins);
   
   // Store electric fields (and very rough errors) in TH3
   for (int ii=0; ii<NDTBINSMODEL; ii++) {
      //h_binDef->Fill(1.,0.,1.01*dtBins[ii],Efields[timeBin][ii]);
      h_binDef->SetBinContent(1,1,ii+1,Efields[timeBin-1][ii]);
      //double error;
      //if (ii != NDTBINSMODEL-1) {
      //	 error = (Efields[timeBin-1][ii] - Efields[timeBin-1][ii+1]) / 2.;
      //} else {
      //	 error = (Efields[timeBin-1][ii-1] - Efields[timeBin-1][ii]) / 2.;
      //}
      h_binDef->SetBinError(1,1,ii+1,Efields_error[timeBin-1][ii]);
      //cout<<"Slice "<< ii+1 <<" E-field: "<< h_binDef->GetBinContent(1,1,ii+1) <<" +/- "<< h_binDef->GetBinError(1,1,ii+1) <<" V/cm"<<endl;
   }
   // Old way, with uniform bin sizes:
   //TH3F* h_binDef = new TH3F("h_binDef","h_binDef",NRBINSMODEL,0,FIDRADIUS*1.01,NPHIBINSMODEL,PHIMIN,PHIMAX*1.01,NDTBINSMODEL,DTMIN,DTMAX*1.01);
   return h_binDef;
}

TH3F* modelBinFrDef(int timeBin = 1, TString histname = "h_binFrDef") {
// Define the edges of the Plan B+ "model bins" (or "voxels") with a TH3.
// Bin contents are set to the proper "Fr" factor for each model bin

   //get a histogram from modelBinDef
   TH3F* h_binFrDef = modelBinDef();
   h_binFrDef->SetName(histname.Data());
   
   //define factors to use
   float Fr_factors[NTIMEBIN][NDTBINSMODEL] = { {0.0093, 0.0086, 0.0078, 0.0075},   // time bin 1
					        {0.01, 0.01, 0.01, 0.01},           // time bin 2
					        {0.0105, 0.0096, 0.009, 0.0081},    // time bin 3
					        {0.0107, 0.0096, 0.0089, 0.0087} }; // time bin 4
   
   // Store factors in TH3
   for (int ii=0; ii<NDTBINSMODEL; ii++) {
      
      h_binFrDef->SetBinContent(1,1,ii+1,Fr_factors[timeBin-1][ii]);
      
      std::cout<<"Slice "<< ii+1 <<" Fr: "<< h_binFrDef->GetBinContent(1,1,ii+1) <<std::endl;
   }
   // Old way, with uniform bin sizes:
   //TH3F* h_binDef = new TH3F("h_binDef","h_binDef",NRBINSMODEL,0,FIDRADIUS*1.01,NPHIBINSMODEL,PHIMIN,PHIMAX*1.01,NDTBINSMODEL,DTMIN,DTMAX*1.01);
   return h_binFrDef;
}

TH3F* spatialBinDef() {
// Return an empty TH3 with the appropriate binning for the spatial PDFs

   float rBins[NRBIN+1];
   for (int ii=0; ii<=NRBIN; ii++) {
      rBins[ii] = (FIDRADIUS - 0)*(double)ii / (NRBIN) + 0;
   }
   
   float phiBins[NPHIBIN+1] = { PHIMIN, -1., 0., 2.25, PHIMAX };

   float dtBins[NDTBIN+1];
   for (int ii=0; ii<=NDTBIN; ii++) {
      dtBins[ii] = (DTMAX - DTMIN)*(double)ii / (NDTBIN) + DTMIN;
      //cout<< dtBins[ii] <<endl;
   }
   //lining up spatial bin edges with model bin edges
   //first 15 bins are 13 us wide, the last 5 bins inside the last model
   //bin are 12 us wide
   //float binWidth1 = 13;
   //float binWidth2 = 12;
   //for (int ii=0; ii<=NDTBIN; ii++) {
      //if(ii<NDTBIN-5) dtBins[ii] = DTMIN + binWidth1 * (double)ii;
      //else dtBins[ii] = 235. + binWidth2 * ((double)ii - (NDTBIN-5));
      //cout<< dtBins[ii] <<endl;
   //}
   
   TH3F* h_spatialBinDef = new TH3F("h_spatialBinDef","h_spatialBinDef",NRBIN,rBins,NPHIBIN,phiBins,NDTBIN,dtBins);
   return h_spatialBinDef;
}

TString timeBinDef(int timeBin=0) {
// Return the file name of the field map corresponding to supplied time bin
// Current definitions are from Lucie, for the preliminary Plan B+ limit:
// https://luxexp.slack.com/archives/run4fields/p1463601349000028

   TString fieldMapFileName;
   switch (timeBin) {
      case 1: fieldMapFileName = "v30_fields_20141006T2125_cp11327_20141006T1924_cp11325.txt";
	      break;
      case 2: fieldMapFileName = "v30_fields_20150226T1943_cp17072.txt";
	      break;
      case 3: fieldMapFileName = "v31_fields_20150722T1117_cp16866.txt";
	      break;
      case 4: fieldMapFileName = "v30_fields_20160222T1702_cp20511.txt";
	      break;
      default: std::cout << "COUT:: Invalid time bin supplied to function timeBinDef. Using time bin 1." << endl;
	       fieldMapFileName = "v30_fields_20141006T2125_cp11327_20141006T1924_cp11325.txt";
   }
   return fieldMapFileName;
}
#endif //MODELBINDEF_H
