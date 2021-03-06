// parameter for background model
#ifndef PARAMBKG_C
#define PARAMBKG_C 1


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
#include "bkgClass.C"
#include "../Parameters.h"
using namespace std;

// expected value 
// in unit of per mass per day activities (per kg per day)
//#define EXPOSURE 337.06
//#define MASSACTIVEVOLUME 250.9 // -Curt, see PRD

double EXPUniform=1.;
double EXP8B=2.e-5; //from wei's simulation    //  .16/250.9;// From Jeremy, 06302016, S1 [0 50], Log10S2 [2 4], DriftTime [40, 300] 
double EXPAccidental=0.0039/100; //from wei's simulation //nExpectedAccidental = 0.0037788 * EXPOSURE; //this is Tomasz's estimate for S2raw > 200 phd. from ImportAccidentalModel.h
double EXPAr37=0.9e-3;//estimate in data Wei. We cannot really distinguish Ar37 or ER abnormally. so I choose this value  
double EXPRnKr=20.e-3*0.01*70*0.67+ 2.99e-3*0.2*70*0.67; //from wei's simulation: 20e-3mBq/kg Rn222 + 2.99 e-3 mBq/kg/day (8.68e3 counts/3.36e4 kg day) Kr85
double EXPComptonBottom==1.e-2;
double EXPComptonRest= 10.e-2;
double EXPKr83=2.e-3;
double EXPXe127;
double EXPneutron; 


//end expect value
// in unit of mDRU (1.e-3 per kg per day per keV)

int paramBkg(){
  bkgCompFull uniform("Uniform"); //default spectrum with uniform energy and uniform spacial distribution in the detector. 
  bkgCompFull ar37("Ar37");
  ar37.expectValue =EXPAR37*EXPOSURE;
  ar37.errValue =ERRAR37*EXPOSURE;
  cout<<ar37.expectValue<<endl;
  ar37.bkg_tb2.Notify();
  return 1;
}

#endif











