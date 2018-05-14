//Script to apply fiducial cut in plan A
//using histogram stored in ROOT file

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

//vars to dump input data into
ULong64_t luxstamp_in;
Float_t radius, phi_in, drift_in;
Float_t spikyS1, s2area, s2area_raw;
Int_t timeBin_in;

TFile *f =TFile::Open("/net/cms26/cms26r0/sjh/lux_bkg_model/planA_bkg_model/tB1_fidCut.root");
TH2D* h_rmax1 = (TH2D*)f->Get("hist");
h_rmax1->SetDirectory(0);
f->Close();

TFile *f2 =TFile::Open("/net/cms26/cms26r0/sjh/lux_bkg_model/planA_bkg_model/tB2_fidCut.root");
TH2D* h_rmax2 = (TH2D*)f2->Get("hist");
h_rmax2->SetDirectory(0);
f2->Close();

TFile *f3 =TFile::Open("/net/cms26/cms26r0/sjh/lux_bkg_model/planA_bkg_model/tB3_fidCut.root");
TH2D* h_rmax3 = (TH2D*)f3->Get("hist");
h_rmax3->SetDirectory(0);
f3->Close();

bool GoldenEvent();

void applyFidCut( const char* filename ){
    
    // Create chain with the original RQ files, containing all the events
    std::string filename_string( filename );
    unsigned ext_pos = filename_string.find_last_of(".");
    std::string type_string = filename_string.substr(0,ext_pos);
    TString outname( (char *)type_string );
    
    TChain *ch1 = new TChain("golden");
    ch1->Add(TString::Format("/cms26r0/sjh/lux_bkg_model/planA_bkg_model/planA_goldenFiles_noFidCut/data/%s",filename));
    
    // Open the root file where golden events will be written
    TFile *hfile =
    new TFile("/cms26r0/sjh/lux_bkg_model/planA_bkg_model/planA_BkgModelInputs/"+outname+"_fidCut.root","RECREATE");
    
    // Now the new TTree, containing only golden events
    TTree *golden = new TTree("platinum","Distilled TTree, golden events with fiducial cut applied");

    Int_t nentries1 = (Int_t)ch1->GetEntries();
    cout << nentries1 << endl;
    
    // Associate each RQ to the corresponding chain branch
   // ch1->SetBranchAddress("luxstamp_samples", &luxstamp_samples);
    
    ch1->SetBranchAddress("luxstamp", &luxstamp_in);
    ch1->SetBranchAddress("radius", &radius);
    ch1->SetBranchAddress("phi", &phi_in);
    ch1->SetBranchAddress("drift", &drift_in);
    ch1->SetBranchAddress("spikyS1", &spikyS1);
    ch1->SetBranchAddress("s2area", &s2area);
    ch1->SetBranchAddress("s2area_raw", &s2area_raw);
    ch1->SetBranchAddress("timeBin", &timeBin_in);
    
    bool gold;
    
    //These are the RQs we want to keep in the new TTree
    Float_t r, phi, drift, S1, log10S2, rawS2;
    ULong64_t luxstamp;
    Int_t timeBin;
    
    golden->Branch("luxstamp",&luxstamp,"luxstamp/l");
    golden->Branch("log10S2",&log10S2,"log10S2/F");
    golden->Branch("S1",&S1,"S1/F");
    golden->Branch("rawS2",&rawS2,"rawS2/F");
    golden->Branch("r",&r,"r/F");
    golden->Branch("phi",&phi,"phi/F");
    golden->Branch("drift",&drift,"drift/F");
    golden->Branch("timeBin",&timeBin,"timeBin/I");
    
    
    // main cycle, through all the events
    for(Int_t evt=0; evt<nentries1; evt++){
    
       if ( evt % 10000 == 0) cout << evt << endl;
        
       ch1->GetEntry(evt);
       /*
       cout<<"inputs:"<<endl;
       cout<<"r: "<< radius <<endl;
       cout<<"phi: "<< phi_in <<endl;
       cout<<"drift: "<< drift_in <<endl;
       cout<<"timeBin: "<< timeBin_in <<endl;
       */
       gold = GoldenEvent();
       
       if (gold) {
       
          //cout<<"Event passed!"<<endl;
          luxstamp = luxstamp_in;
          r = radius;
          phi = phi_in;
          drift = drift_in;
          S1 = spikyS1;
          log10S2 = log10(s2area);
          rawS2 = s2area_raw;
          timeBin = timeBin_in;
          
          // Now save this event to the tree
          golden->Fill();   		
       }
    }//end of event loop
    
    //Write file and close
    golden->Write();

    hfile->Close();
    
} //end of function

bool GoldenEvent() {
   
    if( s2area_raw > 175. ){ //raw S2 threshold cut
       if( drift_in > 30. && drift_in < 295. ){
          if( timeBin_in == 1 && 
          radius < h_rmax1->GetBinContent( h_rmax1->GetXaxis()->FindBin(phi_in) ,h_rmax1->GetYaxis()->FindBin(drift_in) )  ){
             return true;
           }else if( timeBin_in == 2 && 
          radius < h_rmax2->GetBinContent( h_rmax2->GetXaxis()->FindBin(phi_in) ,h_rmax2->GetYaxis()->FindBin(drift_in) )  ){
             return true;
           }else if( timeBin_in == 3 && 
          radius < h_rmax3->GetBinContent( h_rmax3->GetXaxis()->FindBin(phi_in) ,h_rmax3->GetYaxis()->FindBin(drift_in) )  ){
            
             //cout<<"phi bin "<<h_rmax->GetXaxis()->FindBin(phi_in)<<endl;
             //cout<<"drift bin "<<h_rmax->GetYaxis()->FindBin(drift_in)<<endl;
             //cout<<"bin content "<<h_rmax->GetBinContent( h_rmax->GetXaxis()->FindBin(phi_in) ,h_rmax->GetYaxis()->FindBin(drift_in) )<<endl;
          
             return true;
              }
       }
    }
    
    return false;
    
}
 
