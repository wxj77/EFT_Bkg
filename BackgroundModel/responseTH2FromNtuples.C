//Macro to generate RooHistPdfs in log10S2 v S1 and z_cm v rSquared
// 5 histograms (S1,S2 and r,z taken to be independent).

#include "../Parameters.h"
#include "modelBinDef.h"

//using namespace RooFit;

//TCanvas *c1 = new TCanvas("c1","c1");
//c1->SetLogy(0);
const double minDt=DTMIN;
const double maxDt=DTMAX;
const double maxRadius=FIDRADIUS;
const double minPhi=PHIMIN;
const double maxPhi=PHIMAX;
const double minRawS2_phe=S2RAWTHRESHOLD;
const double minS1=S1MIN;
const double maxS1=S1MAX;
const double minlog10S2 = LOG10S2MIN;
const double maxlog10S2 = LOG10S2MAX;
const int nBinsS1=NS1BIN;
const int nBinslog10S2=NLOGS2BIN;
const int nBinsDt=NDTBIN;
const int nBinsRadius=NRBIN;
const int nBinsPhi=NPHIBIN;
const int livedays=EXPOSURE;

//TString basedir(DATABASEDIR);
TString basedir("/net/cms26/cms26r0/sjh/lux_bkg_model/planA_bkg_model/planA_BkgModelInputs/");

//TCut FV(TString::Format("(drift>%f&&drift<%f&&r<%f&&phi>%f&&phi<%f&&rawS2>%f&&S1>%f&&S1<%f)",
//                        minDt,maxDt,maxRadius,minPhi,maxPhi,minRawS2_phe,minS1,maxS1));

TString smoothOpt("k5b");

//this is crying out for a struct/class treatment, but anyway...
//First make plain arrays[nFiles] for the known input files and weightings
//Then make lists of contained files for each population

// Notes from Brian:
// if contains "All" -> includes everything except early-chain Th
// if contains "Ex" -> includes all sources except PMTs

const Int_t nFiles=10;
TString filenames[nFiles]=
{
"Ex_All",
"FCS_Co60",
"FCS_Ra226",
"FCS_Tl208",
"PMTB_All",
"PMTT_All",
"Xe_Kr85",
"Xe_Rn220",
"Xe_Rn222",
"Xe_Xe127"
};
Double_t pfcEfficiency=0.969; //result of hand scanning
// 3% of waveforms weren't classified as golden when they were
Double_t simExposure = 3.*3400.;
Double_t livetimeRatio=EXPOSURE/simExposure;
Double_t weights[nFiles]=
{
  //need to update these factors
  pfcEfficiency*livetimeRatio,
  pfcEfficiency*livetimeRatio*1.6,
  pfcEfficiency*livetimeRatio*1.26,
  pfcEfficiency*livetimeRatio*0.35*0.22,
  pfcEfficiency*livetimeRatio,
  
  pfcEfficiency*livetimeRatio,
  pfcEfficiency*livetimeRatio*0.1*(2.4/0.19),//a new estimate for run4 from Scott Hertel, 2.4 +/- 1.2 events/kg/day/keV, compared to Adam's 0.19 mDRU in Run3
  pfcEfficiency*livetimeRatio*0.2*0.0,
  //^^^ 0.0 because it's nearly all removed in the high field region (Scott Hertel DDB252)
  pfcEfficiency*livetimeRatio*0.63*7./17.9, //0.63*7./17.9 is compensating for Bi-214 not being vetoed in the simulation when it is, by Po-214, in reality and for a assuming 17.9 mBq rather than 7 in BG paper.
  
  pfcEfficiency*livetimeRatio*0.3*13.1/7.6 // 13.1/7.6 is compensating for wrong branching ratio of 5.2 keV in GEANT (DDB348, Huang APS 2015)
};
//we're going to make populations with various subsets of the input files above, using their weights
//each pdf has a list of input files that contribute to it

const int nPop=3;
const int nc[nPop]={3,4,2}; //number inputs for each pdf
TArrayI componentIds[nPop]; // 
TString labels[nPop];  //name of pdf

labels[0]="RnKr";
int betaComA[nc[0]]={6,7,8};
TArrayI betaCom(nc[0],betaComA);
componentIds[0]=betaCom;

labels[1]="ComptonBottom";
int comptonBottomComA[nc[1]]={1,2,3,4};
TArrayI comptonBottomCom(nc[1],comptonBottomComA);
componentIds[1]=comptonBottomCom;

labels[2]="ComptonRest";
int comptonRestComA[nc[2]]={0,5};
TArrayI comptonRestCom(nc[2],comptonRestComA);
componentIds[2]=comptonRestCom;

Float_t r, drift, phi, S1, log10S2, rawS2;
Int_t timeBin;

TH3F* h_binDef = modelBinDef();
int binMultiplier = 2;
//const int nModelBinsX = h_binDef->GetNbinsX();
//const int nModelBinsY = h_binDef->GetNbinsY();
//const int nModelBinsZ = h_binDef->GetNbinsZ();

const int nModelBinsX = NRBINSMODEL;
const int nModelBinsY = NPHIBINSMODEL;
const int nModelBinsZ = NDTBINSMODEL;

void makeTH2(const int popInd, const int time_bin)
{
  cout<<"Creating "<<labels[popInd]<<" in time bin "<<time_bin<<"..."<<endl;
  TDirectory *d=gDirectory;
  //Delcare the histograms to fill:
  TH2F* h_S1vslog10S2_array[nModelBinsX][nModelBinsY][nModelBinsZ];
  for (int ix = 0; ix < nModelBinsX; ix++) {
     for (int iy = 0; iy < nModelBinsY; iy++) {
        for (int iz = 0; iz < nModelBinsZ; iz++) {
	   // Naming convention: h_S1vslog10S2_TimeBin<timebin>__R<rbin>_PHI<phibin>_DT<dtbin>
	   // where each bin number goes from 1 to Nbins (inclusive)
	   TString histname = TString::Format("h_S1vslog10S2_TimeBin%d__R%d_PHI%d_DT%d",time_bin,ix+1,iy+1,iz+1);
	   h_S1vslog10S2_array[ix][iy][iz] = new TH2F(histname.Data(),histname.Data(),binMultiplier*NS1BIN,S1MIN,S1MAX,binMultiplier*NLOGS2BIN,LOG10S2MIN,LOG10S2MAX);
	   cout << "Declared histogram " << histname.Data() << endl;
	}
     }
  }
  
  //Loop over the files that contribute to this bkg type
  TArrayI comIds=componentIds[popInd];
  for(int i=0;i<comIds.GetSize();i++)
  {
    //cout<<"Opening "<<basedir+TString("golden")+filenames[comIds[i]]+TString("fidCut.root")<<endl;
    TFile *f =TFile::Open(basedir+TString("golden")+filenames[comIds[i]]+TString("_fidCut.root"));
    d->cd();
    TTree* golden = (TTree*)f->Get("platinum");
    
    TCut FV(TString::Format("(drift>%f&&drift<%f&&r<%f&&phi>%f&&phi<%f&&rawS2>%f&&S1>%f&&S1<%f&&timeBin==%d)",
         minDt,maxDt,maxRadius,minPhi,maxPhi,minRawS2_phe,minS1,maxS1,time_bin));
    
    golden->SetBranchAddress("r",&r);
    golden->SetBranchAddress("drift",&drift);
    golden->SetBranchAddress("phi",&phi);
    golden->SetBranchAddress("S1",&S1);
    golden->SetBranchAddress("log10S2",&log10S2);
    golden->SetBranchAddress("rawS2",&rawS2);
    golden->SetBranchAddress("timeBin",&timeBin);
    Int_t nentries = (Int_t)golden->GetEntries();
    for(Int_t evt=0; evt<nentries; evt++){
       golden->GetEntry(evt);
       
       if( FV.GetTitle() ){ //analysis cuts
          
          //Determine what model bin we are in
          int rbin,phibin,dtbin;
          int globalBin = h_binDef->FindBin(r,phi,drift);
          h_binDef->GetBinXYZ(globalBin,rbin,phibin,dtbin);
          h_S1vslog10S2_array[rbin-1][phibin-1][dtbin-1]->Fill( S1, log10S2, weights[comIds[i]] );
       }
       
    }
    //cout<<"Bkg: "<<labels[popInd]<<"\t"<<"Filename: "<<filenames[comIds[i]]<<"\t"<<"Weight: "<<weights[comIds[i]]<<endl;
    //cout<<golden->Draw("r:phi:drift>>+hist",TString::Format("%f*%s",weights[comIds[i]], FV.GetTitle()),"colz")<<endl;
    f->Close();
  }
  
  
  cout<<"Opening output file..."<<endl;
  TFile *outfile = new TFile(TString::Format("Bkg_S1Log10S2_TH2F_%s_TB%d.root",labels[popInd].Data(),time_bin),"RECREATE");
  //TH3F *spatial_hist = (TH3F*)hist.Clone("spatial_hist");
  TH2F* h_S1vslog10S2_clone_array[nModelBinsX][nModelBinsY][nModelBinsZ]; //have to do this to make it work...
  
  cout<<"Saving Histograms..."<<endl;
  TCanvas* cans[nModelBinsX][nModelBinsY][nModelBinsZ];
  for (int ix = 0; ix < nModelBinsX; ix++) {
     for (int iy = 0; iy < nModelBinsY; iy++) {
        for (int iz = 0; iz < nModelBinsZ; iz++) {
           
           h_S1vslog10S2_array[ix][iy][iz]->Smooth( 1,smoothOpt );
	   h_S1vslog10S2_clone_array[ix][iy][iz] = (TH2F*)h_S1vslog10S2_array[ix][iy][iz]->Clone();
	   
	   
	   TString canName = TString::Format("c_TimeBin%d__R%d_PHI%d_DT%d",time_bin,ix+1,iy+1,iz+1);
	   cans[ix][iy][iz] = new TCanvas(canName.Data(),canName.Data());
	   h_S1vslog10S2_clone_array[ix][iy][iz]->SetTitle( TString::Format("%s_TimeBin%d__R%d_PHI%d_DT%d",labels[popInd].Data(),time_bin,ix+1,iy+1,iz+1) );
	   h_S1vslog10S2_clone_array[ix][iy][iz]->Draw("colz");
	   h_S1vslog10S2_clone_array[ix][iy][iz]->Write();
	   cans[ix][iy][iz]->Write();
	   
	}
     }
  }
  cout<<"Histograms saved"<<endl;
  
  //bgws->factory(TString::Format("%s_expectation[%f]",(char*)(labels[popInd]),s1ls2hist.Integral())); //expected number of counts
  //bgws->var(TString::Format("%s_expectation",(char*)(labels[popInd])))->Print();
  
  outfile->Close();
  cout<<"Done"<<endl;
  cout<<endl;
}

void responseTH2FromNtuples()
{
  
  // loop over pdfs to be created
  // and loop over time bins
  // nine files get created: one for each time bin and pdf type
  for (int i=0;i<nPop;i++){ //nPop
    
    for (int j=0;j<NTIMEBIN;j++){ //NTIMEBIN
       makeTH2( i,j+1 );
    }
    
  }
  
  gApplication->Terminate();
}
