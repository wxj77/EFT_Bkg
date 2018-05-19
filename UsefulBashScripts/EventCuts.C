#ifndef EVENTCUTS_C
#define EVENTCUTS_C 1


//c lib
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


//custom libs
//class bkgComp;
//class bkgCompFull;

using namespace std;




bool EventCuts(double * x, double *par=NULL){
  double S1_area=x[0];
  double S1_area_z_cor=x[1];
  double S1_spiky_z_cor=x[2];
  double S2_area_raw =x[3];
  double S2_area_z_cor=x[4];
  
//  notes  from Evan Pease for Run4 data cuts
//  # S1, S2 cuts...
// Cuts on energy (less than 50 keVee):
 bool value=0;
 bool value0=0, value1=0, value2=0, value3=0,value4=0;
 if  (S2_area_z_cor < (69419.7-(192.5787*S1_area_z_cor))) value1=1;

//# Cuts on S1 size (note: if you don't have S1 spike count, just require S1_area_z_cor >= 1 and < 300):
 if ( (S1_area_z_cor<300) && ((S1_spiky_z_cor>=1) || ((S1_spiky_z_cor==0)*(S1_area>20))) ) value2=1;

//# Upper bounds on S2 area:
 if ( (((S2_area_z_cor<(pow(10.,4.5))) && (S1_area_z_cor>=50)) || ((log10(S2_area_z_cor)<(3.326*(pow(S1_area_z_cor, (-0.084))) + log10(S1_area_z_cor))) && (S1_area_z_cor<50))) ) value3=1;

//# Lower bounds on S2 area:
 if ( (((S2_area_z_cor>pow(10.,3)) && (S1_area_z_cor>=50)) || ((log10(S2_area_z_cor)>(0.8823*pow(S1_area_z_cor,(0.096)) + log10(S1_area_z_cor))) && (S1_area_z_cor<50))) ) value4=1;

//# Cuts on S2_raw:
 if (S2_area_raw > 200) value0=1; 
  
  value = value1 && value2 && value3 &&value4 && value0;
  return value;
}
#endif
