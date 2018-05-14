//parameters of the set histogram
#ifndef PARAMBKGGEN_C
#define PARAMBKGGEN_C 1

#include <iostream>
#include <cstdlib>

using namespace std;
/*
int NbinsS1=200; 
double MinVS1=1; //S1 does not support non integer binning now
double BinWS1=1.; //S1 does not support non integer binning now
double MaxVS1=MinVS1+BinWS1*(NbinsS1); 

int NbinsS2=200;
double MinVS2=200;
double BinWS2=200.;
double MaxVS2=MinVS2+BinWS2*(NbinsS2);

int NbinsLog10S2=200;
double MinVLog10S2=2;
double BinWLog10S2=0.01;
double MaxVLog10S2=MinVLog10S2+BinWLog10S2*(NbinsLog10S2);

void paramBkgGen(){
cout<<"S1 bins: "<<endl;
cout<<"N: "<<NbinsS1<<"\tmin: "<<MinVS1<<"\tmax: "<<MaxVS1<<endl;

cout<<"S2 bins: "<<endl;
cout<<"N: "<<NbinsS2<<"\tmin: "<<MinVS2<<"\tmax: "<<MaxVS2<<endl;

cout<<"Log10S2 bins: "<<endl;
cout<<"N: "<<NbinsLog10S2<<"\tmin: "<<MinVLog10S2<<"\tmax: "<<MaxVLog10S2<<endl;
}

*/

#endif


#ifndef PARAMETERS_H
#define PARAMETERS_H

// These are global variables and #define imperatives
// Include this before your other code.
// Use in moderation

//Define number of model bins:
#define NRBINSMODEL 1
#define NPHIBINSMODEL 1
#define NDTBINSMODEL 4

#define FIDRADIUS 20.5 //slightly larger than largest value of cut (which is 20.4089)
#define NRBIN 12

#define S2RAWTHRESHOLD 200
#define NRECOILE 55

// defined by Z = 5.6 + 48.72 - 0.1501*drift  [cm] with drift [305,38] us
//#define NZBIN 20
//#define ZMIN 8.54
//#define ZMAX 48.6

// temporary drift time bins
#define NDTBIN 20
#define DTMIN 40
#define DTMAX 300

#define S1MIN 1
#define S1MAX 200
#define NS1BIN 199

#define LOG10S2MIN 2.0
#define LOG10S2MAX 4.0
#define NLOGS2BIN 200

#define LS2MIN 200
#define LS2MAX 40200
#define NLS2BIN 200

#define PHIMIN -3.15
#define PHIMAX 3.15
#define NPHIBIN 4

#define NFASTNESTEVT 1e5
#define NSMOOTH 1
#define MINRECOILENERGY 1.1
#define USELOGSCAN true
#define SCALENRS2 1.0

#define MATHPI 3.14159265359
#define EXPOSURE 337.06
#define MASSACTIVEVOLUME 250.9 // -Curt, see PRD

#define NTIMEBIN 4

// fractions of acitve volume for fid mass for each time bin - updated 5.31.16, sjh
#define FIDFRACTION1 0.420
#define FIDFRACTION2 0.427
#define FIDFRACTION3 0.395
#define FIDFRACTION4 0.392

#define FIDMASS1 FIDFRACTION1*MASSACTIVEVOLUME
#define FIDMASS2 FIDFRACTION2*MASSACTIVEVOLUME
#define FIDMASS3 FIDFRACTION3*MASSACTIVEVOLUME
#define FIDMASS4 FIDFRACTION3*MASSACTIVEVOLUME

// sjh - Numbers from Evan for prelim limit:
#define TIMEBIN1LIVETIME 46.766
#define TIMEBIN2LIVETIME 46.731
#define TIMEBIN3LIVETIME 91.552
#define TIMEBIN4LIVETIME 146.923

//#define DATABASEDIR "/net/cms26/cms26r0/sjh/lux_bkg_model/planA_bkg_model/planA_BkgModelInputs/"
#define DATABASEDIR "./bkgModelInputs/"

#endif //PARAMETERS_H
