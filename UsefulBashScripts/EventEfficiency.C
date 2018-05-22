#ifndef EVENTEFFICIENCY_C
#define EVENTEFFICIENCY_C 1


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




double EventEfficiency(double * x, double *par=NULL){
  double S1_area=x[0];
  double S1_area_z_cor=x[1];
  double S1_spiky_z_cor=x[2];
  double S2_area_raw =x[3];
  double S2_area_z_cor=x[4];
  
//  notes  from Evan Pease for Run4 data cuts
//  # S1, S2 cuts...
// Cuts on energy (less than 50 keVee):
  double value=0;
//  bool value0=0, value1=0, value2=0, value3=0,value4=0;  
//  value = value1 && value2 && value3 &&value4 && value0;

  value=1; //Need to change this during real run.
  return value;
}
#endif
