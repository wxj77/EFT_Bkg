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
#include "../../Parameters.h"

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

TFile *f4 =TFile::Open("../../FidCutScript/tB4_fidCut.root");
TH2D* h_rmax4 = (TH2D*)f4->Get("hist");
h_rmax4->SetDirectory(0);
f4->Close();

bool GoldenEvent(int);

void filter( char* type, int time_bin ){
    
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
    }else if( filterType.EqualTo("Ex_Th232") ){
       ch1->Add(timeBinDir+"CRYOSTAT_Th232E_summary.root");
       ch1->Add(timeBinDir+"FCSHIELD_Th232E_summary.root");
       ch1->Add(timeBinDir+"FIELDGRID_Th232E_summary.root");
       ch1->Add(timeBinDir+"FIELDRING_Th232E_summary.root");
       ch1->Add(timeBinDir+"HDPE_Th232E_summary.root");
       ch1->Add(timeBinDir+"MYLAR_Th232E_summary.root");
       ch1->Add(timeBinDir+"PMTMOUNT_Th232E_summary.root");
       ch1->Add(timeBinDir+"PTFE_Th232E_summary.root");
       ch1->Add(timeBinDir+"REFLECTOR_Th232E_summary.root");
       ch1->Add(timeBinDir+"THERMALSHIELD_Th232E_summary.root");
       ch1->Add(timeBinDir+"TOPSHIELD_Th232E_summary.root");
    }else if( filterType.EqualTo("FCS_Co60") ){
        ch1->Add(timeBinDir+"FCSHIELDPLANE_Co60_summary.root");
    }else if( filterType.EqualTo("FCS_Pb212") ){
        ch1->Add(timeBinDir+"FCSHIELDPLANE_Pb212_summary.root");
    }else if( filterType.EqualTo("FCS_Ra226") ){
        ch1->Add(timeBinDir+"FCSHIELDPLANE_Ra226_summary.root");
    }else if( filterType.EqualTo("FCS_Tl208") ){
        ch1->Add(timeBinDir+"FCSHIELDPLANE_Tl208_summary.root");
    }else if( filterType.EqualTo("PMTB_All") ){
       ch1->Add(timeBinDir+"PMTB_Co60_summary.root");
       ch1->Add(timeBinDir+"PMTB_K40_summary.root");
       ch1->Add(timeBinDir+"PMTB_Ra226_summary.root");
       ch1->Add(timeBinDir+"PMTB_Th232L_summary.root");
    }else if( filterType.EqualTo("PMTB_Th232") ){
       ch1->Add(timeBinDir+"PMTB_Th232E_summary.root");
    }else if( filterType.EqualTo("PMTT_All") ){
       ch1->Add(timeBinDir+"PMTT_Co60_summary.root");
       ch1->Add(timeBinDir+"PMTT_K40_summary.root");
       ch1->Add(timeBinDir+"PMTT_Ra226_summary.root");
       ch1->Add(timeBinDir+"PMTT_Th232L_summary.root");
    }else if( filterType.EqualTo("PMTT_Th232") ){
       ch1->Add(timeBinDir+"PMTT_Th232E_summary.root");
    }else if( filterType.EqualTo("Xe_Kr85") ){
       ch1->Add(timeBinDir+"XENON_Kr85_summary.root");
    }else if( filterType.EqualTo("Xe_Rn220") ){
       ch1->Add(timeBinDir+"XENON_Rn220_summary.root");
    }else if( filterType.EqualTo("Xe_Rn222") ){
       ch1->Add(timeBinDir+"XENON_Rn222_summary.root");
    }else if( filterType.EqualTo("Xe_Xe127") ){
       ch1->Add(timeBinDir+"XENON_Xe127_summary.root");
    }
    
    // Open the root file where golden events will be written
    TString tb_str = TString::Format("tb%d",time_bin);
    TFile *hfile = new TFile("../bkgModelInputs/golden"+filterType+"_"+tb_str+"_fidCut.root","RECREATE");
    
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
    TH2F* h_rvsz_real = new TH2F("h_R2vsZ_real"," ;r (cm);z (cm)",100,0,26,100,0,60);
    TH2F* h_rvsz_real_cut = new TH2F("h_R2vsZ_real_cut","Real space cut ;r (cm);z (cm)",100,0,26,100,0,60);
    TH2F* h_rvsz_real_gold = new TH2F("h_R2vsZ_real_gold","Events that pass fid cut;r (cm);z (cm)",100,0,26,100,0,60);
    
    TH2F* h_rvsdt_S2 = new TH2F("h_R2vsDT_S2"," ;r_{S2} (cm);-drift (us)",100,0,26,100,-350,0);
    TH2F* h_rvsdt_S2_cut = new TH2F("h_R2vsDT_S2_cut","Real space cut ;r_{S2} (cm);-drift (us)",100,0,26,100,-350,0);
    TH2F* h_rvsdt_S2_gold = new TH2F("h_R2vsDT_S2_gold","Events that pass fid cut;r_{S2} (cm);-drift (us)",100,0,26,100,-350,0);
    
    TH2F* h_xvsy_real = new TH2F("h_XvsY_real"," ;x (cm);y (cm)",100,-23,23,100,-23,23);
    TH2F* h_xvsy_real_gold = new TH2F("h_XvsY_real_gold","Events that pass fid cut;x (cm);y (cm)",100,-23,23,100,-23,23);
    
    TH2F* h_xvsy_S2 = new TH2F("h_XvsY_S2"," ;x_{S2} (cm);y_{S2} (cm)",100,-23,23,100,-23,23);
    TH2F* h_xvsy_S2_gold = new TH2F("h_XvsY_S2_gold","Events that pass fid cut;x_{S2} (cm);y_{S2} (cm)",100,-23,23,100,-23,23);
    
    //////////
    
    Int_t nentries1 = (Int_t)ch1->GetEntries();
    cout << "Chain has " << nentries1 << " entries" << endl;
    
    // Associate each RQ to the corresponding chain branch
    //ch1->SetBranchAddress("luxstamp_samples", &luxstamp_samples);
    ch1->SetBranchAddress("x_cm", &x_cm);
    ch1->SetBranchAddress("y_cm", &y_cm);
    ch1->SetBranchAddress("r_cm", &r_cm);
    ch1->SetBranchAddress("z_cm", &z_cm);
    ch1->SetBranchAddress("phi", &phi_in);
    
    ch1->SetBranchAddress("x_s2_cm", &x_s2_cm);
    ch1->SetBranchAddress("y_s2_cm", &y_s2_cm);
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
    int count = 0;
    for(Int_t evt=0; evt<nentries1; evt++){
       if ( evt % 10000 == 0) cout << evt << endl; 
       ch1->GetEntry(evt);
       
       //Decide whether golden or not:
       gold = GoldenEvent(time_bin);
       
       h_R2vsZ_real->Fill( r_cm, z_cm );
       h_xvsy_real->Fill( x_cm, y_cm );
       h_R2vsDT_S2->Fill( r_s2_cm, -dt_s2 );
       h_xvsy_S2->Fill( x_s2_cm, y_s2_cm );
       
       if( r_cm > 20. && gold ){
          h_R2vsZ_real_cut->Fill( r_cm, z_cm );
          h_R2vsDT_S2_cut->Fill( r_s2_cm, -dt_s2 );
       }
       
       if (gold){
          r = r_s2_cm;
          drift = dt_s2;
          phi = phi_s2;
          S1 = s1c;
          log10S2 = log10( s2c );
          rawS2 = s2_raw; 
          // Now save this event to the tree
          golden->Fill();
	  count++;
	  
	  h_rvsz_real_gold->Fill( r_cm, z_cm );
	  h_xvsy_real_gold->Fill( x_cm, y_cm );
	  h_rvsdt_S2_gold->Fill( r_s2_cm, -dt_s2 );
	  h_xvsy_S2_gold->Fill( x_s2_cm, y_s2_cm );
       }
       
    }
    
    //Write file and close
    golden->Write();
    
    //save histos
    h_R2vsZ_real->Write();
    h_R2vsZ_real_cut->Write();
    h_rvsz_real_gold->Write();
    h_R2vsDT_S2->Write();
    h_R2vsDT_S2_cut->Write();
    h_rvsdt_S2_gold->Write();
    
    h_xvsy_real->Write();
    h_xvsy_real_gold->Write();
    h_xvsy_S2->Write();
    h_xvsy_S2_gold->Write();
    
    hfile->Close();
    cout<<"Saved "<<count<<" golden events"<<endl;
}

bool GoldenEvent(int timeBin_in) {
   
    if( s2_raw > S2RAWTHRESHOLD ){ //raw S2 threshold cut
    
       if( dt_s2 > DTMIN && dt_s2 < DTMAX ){
          
          if( timeBin_in == 1 && r_s2_cm < h_rmax1->GetBinContent( h_rmax1->FindBin(phi_s2,dt_s2) ) ){
             return true;
           }else if( timeBin_in == 2 && r_s2_cm < h_rmax2->GetBinContent( h_rmax2->FindBin(phi_s2,dt_s2) )  ){
             return true;
           }else if( timeBin_in == 3 && r_s2_cm < h_rmax3->GetBinContent( h_rmax3->FindBin(phi_s2,dt_s2) )  ){
             return true;
           }else if( timeBin_in == 4 && r_s2_cm < h_rmax4->GetBinContent( h_rmax4->FindBin(phi_s2,dt_s2) )  ){
           
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
