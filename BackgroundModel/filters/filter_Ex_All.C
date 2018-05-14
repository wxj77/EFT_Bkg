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
Float_t x_s2_cm, y_s2_cm, dt_s2, r_s2_cm, phi_s2;
Float_t s1_raw, s1c, s2_raw, s2c; //, log10S2;

TFile *f =TFile::Open("../../FidCutScript/tB1_fidCut.root");
TH2D* h_rmax1 = (TH2D*)f->Get("hist");
h_rmax1->SetDirectory(0);
f->Close();

TFile *f2 =TFile::Open("../../FidCutScript/tB2_fidCut.root");
TH2D* h_rmax2 = (TH2D*)f2->Get("hist");
h_rmax2->SetDirectory(0);
f2->Close();

TFile *f3 =TFile::Open("../../FidCutScript/tB3_fidCut.root");
TH2D* h_rmax3 = (TH2D*)f3->Get("hist");
h_rmax3->SetDirectory(0);
f3->Close();

bool GoldenEvent(int timeBin_in);

void filter_Ex_All( char* type, int time_bin ){
    
    TString filterType( type );
    TString timeBinDir = TString::Format("../root_output/tb%d/",time_bin);
    // Create chain with the original RQ files, containing all the events
    TChain *ch1 = new TChain("events");
    
    //Load the correct files to the chain based on input name
    //if( filterType = "Ex_All" ){
    if( filterType.EqualTo("Ex_All") ){
       //cryostat w/o Th232E
       ch1->Add(timeBinDir+"CRYOSTAT_K40_summary.root");
       ch1->Add(timeBinDir+"CRYOSTAT_Ra226_summary.root");
       ch1->Add(timeBinDir+"CRYOSTAT_Sc46_summary.root");
       ch1->Add(timeBinDir+"CRYOSTAT_Th232L_summary.root");
       //FCSHIELD w/o Th232E --  maybe an error here?
       ch1->Add(timeBinDir+"FCSHIELD_Co60_summary.root");
       ch1->Add(timeBinDir+"FCSHIELD_Ra226_summary.root");
       ch1->Add(timeBinDir+"FCSHIELD_Th232L_summary.root");
       //FIELDGRID
       ch1->Add(timeBinDir+"FIELDGRID_Co60_summary.root");
       ch1->Add(timeBinDir+"FIELDGRID_K40_summary.root");
       ch1->Add(timeBinDir+"FIELDGRID_Ra226_summary.root");
       ch1->Add(timeBinDir+"FIELDGRID_Th232L_summary.root");
       //FIELDRING
       ch1->Add(timeBinDir+"FIELDRING_Co60_summary.root");
       ch1->Add(timeBinDir+"FIELDRING_Ra226_summary.root");
       ch1->Add(timeBinDir+"FIELDRING_Th232L_summary.root");
       //HDPE
       ch1->Add(timeBinDir+"HDPE_Ra226_summary.root");
       ch1->Add(timeBinDir+"HDPE_Th232L_summary.root");
       //MYLAR
       ch1->Add(timeBinDir+"MYLAR_K40_summary.root");
       ch1->Add(timeBinDir+"MYLAR_Ra226_summary.root");
       ch1->Add(timeBinDir+"MYLAR_Th232L_summary.root");
       //PMTMOUNT
       ch1->Add(timeBinDir+"PMTMOUNT_Co60_summary.root");
       ch1->Add(timeBinDir+"PMTMOUNT_Ra226_summary.root");
       ch1->Add(timeBinDir+"PMTMOUNT_Th232L_summary.root");
       //PTFE and REFLECTOR
       ch1->Add(timeBinDir+"PTFE_Ra226_summary.root");
       ch1->Add(timeBinDir+"PTFE_Th232L_summary.root");
       ch1->Add(timeBinDir+"REFLECTOR_Ra226_summary.root");
       ch1->Add(timeBinDir+"REFLECTOR_Th232L_summary.root");
       //THERMALSHIELD
       ch1->Add(timeBinDir+"THERMALSHIELD_K40_summary.root");
       ch1->Add(timeBinDir+"THERMALSHIELD_Ra226_summary.root");
       ch1->Add(timeBinDir+"THERMALSHIELD_Th232L_summary.root");
       //TOPSHIELD
       ch1->Add(timeBinDir+"TOPSHIELD_Co60_summary.root");
       ch1->Add(timeBinDir+"TOPSHIELD_Ra226_summary.root");
       ch1->Add(timeBinDir+"TOPSHIELD_Th232L_summary.root");
    }
    
    
    // Open the root file where golden events will be written
    TString tb_str = TString::Format("tb%d",time_bin);
    TFile *hfile = new TFile("../bkgModelInputs/golden"+filterType+"_"+tb_str+".root","RECREATE");
    
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
    
    ch1->SetBranchAddress("x_s2_cm", &x_s2_cm);
    ch1->SetBranchAddress("y_s2_cm", &y_s2_cm);
    //ch1->SetBranchAddress("z_cm", &z_cm);
    ch1->SetBranchAddress("dt_s2", &dt_s2);
    ch1->SetBranchAddress("r_s2_cm", &r_s2_cm);
    ch1->SetBranchAddress("phi_s2", &phi_s2);
    ch1->SetBranchAddress("s1_raw", &s1_raw);
    ch1->SetBranchAddress("s1c", &s1c);
    ch1->SetBranchAddress("s2_raw", &s2_raw);
    ch1->SetBranchAddress("s2c", &s2c);
    //ch1->SetBranchAddress("log10S2", &log10S2);
    
    bool gold;
    
    //These are the RQs we want to keep in the new TTree
    Float_t r, drift, phi, S1, log10S2, rawS2;
    
    golden->Branch("r",&r,"r/F");
    //golden->Branch("z",&z,"z/F");
    golden->Branch("drift",&drift,"drift/F");
    golden->Branch("phi",&phi,"phi/F");
    golden->Branch("S1",&S1,"S1/F");
    golden->Branch("log10S2",&log10S2,"log10S2/F");
    golden->Branch("rawS2",&rawS2,"rawS2/F");
    
    // main cycle, through all the events
    for(Int_t evt=0; evt<nentries1; evt++){
    
       if ( evt % 10000 == 0) cout << evt << endl;
        
       ch1->GetEntry(evt);
        
       gold = GoldenEvent(time_bin);
       
       if (gold) {
           
           r = r_s2_cm;
           //z = z_cm;
           drift = dt_s2;
           phi = phi_s2;
           
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

bool GoldenEvent(int timeBin_in) {
   
    if( s2_raw > 175. ){ //raw S2 threshold cut
       if( dt_s2 > 30. && dt_s2 < 295. ){
          if( timeBin_in == 1 && 
          r_s2_cm < h_rmax1->GetBinContent( h_rmax1->GetXaxis()->FindBin(phi_s2) ,h_rmax1->GetYaxis()->FindBin(dt_s2) )  ){
             return true;
           }else if( timeBin_in == 2 && 
          r_s2_cm < h_rmax2->GetBinContent( h_rmax2->GetXaxis()->FindBin(phi_s2) ,h_rmax2->GetYaxis()->FindBin(dt_s2) )  ){
             return true;
           }else if( timeBin_in == 3 && 
          r_s2_cm < h_rmax3->GetBinContent( h_rmax3->GetXaxis()->FindBin(phi_s2) ,h_rmax3->GetYaxis()->FindBin(dt_s2) )  ){
            
             //cout<<"phi bin "<<h_rmax->GetXaxis()->FindBin(phi_in)<<endl;
             //cout<<"drift bin "<<h_rmax->GetYaxis()->FindBin(drift_in)<<endl;
             //cout<<"bin content "<<h_rmax->GetBinContent( h_rmax->GetXaxis()->FindBin(phi_in) ,h_rmax->GetYaxis()->FindBin(drift_in) )<<endl;
          
             return true;
              }
       }
    }
    
    return false;
    
}
/*
bool GoldenEvent() {
    
    if( s2_raw > 175. ){ //raw S2 threshold cut   
       return true;
    }else{
    return false;
    }
    
}
*/
