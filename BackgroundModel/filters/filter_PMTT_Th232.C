#include <TROOT.h>
#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TMath.h>
#include <iostream>
#include <fstream>

using namespace std;

Float_t x_cm, y_cm, z_cm, r_cm, phi_in;
Float_t s1_raw, s1c, s2_raw, s2c; //, log10S2;

bool GoldenEvent();

void filter_PMTT_Th232_planB(){
    
    // Create chain with the original RQ files, containing all the events
    TChain *ch1 = new TChain("events");
    
    //cryostat w/o Th232E
    ch1->Add("/cms26r0/sjh/lux_bkg_model/planB_bkg_model/root_output/PMTT_Th232E_summary.root");
    
    
    //    TChain *ch1  = (TChain*)gROOT->FindObject("events");
    
    // Open the root file where golden events will be written
    TFile *hfile = new TFile("/cms26r0/sjh/lux_bkg_model/planB_bkg_model/planB_BkgModelInputs/goldenPMTT_Th232.root","RECREATE");
    
    // Now the new TTree, containing only golden events
    TTree *golden = new TTree("golden","Distilled TTree, containing only golden events");

    // Now the n-tuple and histogram for the important information
  //  TNtuple *ntuple = new TNtuple("ntuple","Radius,Z_cm,Phi,S1Area,log10S2,RawS2","r:z:phi:S1:log10S2:rawS2");

    //TH1F *S1Area = new TH1F("S1Area", "S1Area", 50, 0, 250);
   // TH1F *SS1Area = new TH1F("SS1Area", "SS1Area", 50, 0, 250);
    //TH1F *S2Area = new TH1F("S2Area", "S2Area", 250, 0, 8000);
    //TH1F *ZDrift = new TH1F("ZDrift", "ZDrift", 350, 0, 350);
    //TH1F *Radius = new TH1F("Radius", "Radius", 26, 0, 26);
    //TH1F *RadiusR = new TH1F("RadiusR", "Radius Renormalized", 26, 0, 26);   

    Int_t nentries1 = (Int_t)ch1->GetEntries();
    cout << nentries1 << endl;
    
    // Associate each RQ to the corresponding chain branch
   // ch1->SetBranchAddress("luxstamp_samples", &luxstamp_samples);
    
    ch1->SetBranchAddress("x_cm", &x_cm);
    ch1->SetBranchAddress("y_cm", &y_cm);
    ch1->SetBranchAddress("z_cm", &z_cm);
    ch1->SetBranchAddress("r_cm", &r_cm);
    ch1->SetBranchAddress("phi", &phi_in);
    ch1->SetBranchAddress("s1_raw", &s1_raw);
    ch1->SetBranchAddress("s1c", &s1c);
    ch1->SetBranchAddress("s2_raw", &s2_raw);
    ch1->SetBranchAddress("s2c", &s2c);
    //ch1->SetBranchAddress("log10S2", &log10S2);
    
    bool gold;
    
    //These are the RQs we want to keep in the new TTree
    Float_t r, z, phi, S1, log10S2, rawS2;
    
    golden->Branch("r",&r,"r/F");
    golden->Branch("z",&z,"z/F");
    golden->Branch("phi",&phi,"phi/F");
    golden->Branch("S1",&S1,"S1/F");
    golden->Branch("log10S2",&log10S2,"log10S2/F");
    golden->Branch("rawS2",&rawS2,"rawS2/F");
    
    // main cycle, through all the events
    for(Int_t evt=0; evt<nentries1; evt++){
    
       if ( evt % 10000 == 0) cout << evt << endl;
        
       ch1->GetEntry(evt);
        
       gold = GoldenEvent();
       
       if (gold) {
           
           r = r_cm;
           z = z_cm;
           phi = phi_in;
           
           S1 = s1c;
           log10S2 = log10( s2c );
           rawS2 = s2_raw;
           
           // Now save this event to the tree
      	 //  ntuple->Fill(r,z,phi,S1,log10S2,rawS2);
           golden->Fill();
	   		
       }
    }
    
    //Write file and close
    golden->Write();
   // ntuple->Write();  

    hfile->Close();
}


bool GoldenEvent() {
    
    if( s2_raw > 150. ){ //raw S2 threshold cut
    
       return true;
       
    }else{
    
    return false;
    
    }
    
}
 
