//how to run ScaleAllToOne("test.root", "all")
//ScaleAllToOne("test.root", "total")

//option: 
//  "all":      scale each of DT1, DT2, DT3, DT4 to one
//  "total":    scale DT1, DT2, DT3, DT4 so that the sum is one.

//bash script to scale all
//for f in *; do root -q -b -l "../../UsefulBashScripts/ScaleAllToOne.C(\"$f\", \"total\")"; done
//
       
#ifndef SCALEALLTOONE_C
#define SCALEALLTOONE_C 1


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
#include "TH1F.h"
#include "TFile.h"



using namespace std;

int ScaleAllToOne(TString datafile="test.root", TString option="all"){
  TFile* in_file = new TFile(datafile.Data(), "update");
  TH2F* DT1 = (TH2F*) in_file->Get("DT1");
  TH2F* DT2 = (TH2F*) in_file->Get("DT2");
  TH2F* DT3 = (TH2F*) in_file->Get("DT3");
  TH2F* DT4 = (TH2F*) in_file->Get("DT4");
  double integ1 = DT1->Integral();
  double integ2 = DT2->Integral();
  double integ3 = DT3->Integral();
  double integ4 = DT4->Integral();
  double total = integ1 + integ2 + integ3 + integ4 ;
  cout << "nunmber of events in each drift bin are:\n"<<"1: "<<integ1<<"\t"<<"2: "<<integ2<<"\t"<<"3: "<<integ3<<"\t"<<"4: "<<integ4<<"\t"<<"total: "<<total<<"\t"<<endl;
  cout << "frac of events in each drift bin are:\n"<<"1: "<<integ1/total<<"\t"<<"2: "<<integ2/total<<"\t"<<"3: "<<integ3/total<<"\t"<<"4: "<<integ4/total<<"\t"<<endl;
  if (1){
    DT1->Scale(1./integ1);
    DT2->Scale(1./integ2);
    DT3->Scale(1./integ3);    
    DT4->Scale(1./integ4);
  }
  if (option=="total"){
    DT1->Scale(integ1/total);
    DT2->Scale(integ2/total);
    DT3->Scale(integ3/total);    
    DT4->Scale(integ4/total);
  }
  in_file->Write();  
  in_file->Close();
  return 1;
}
#endif

