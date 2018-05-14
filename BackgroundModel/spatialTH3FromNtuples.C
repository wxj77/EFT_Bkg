//Macro to create spatial histograms of simulated bkg events

#include "../Parameters.h"
#include "../EventEfficiency.h"
#include "../BandCuts.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TArrayI.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "Smooth3D.h"
#include "modelBinDef.h"

//using namespace RooFit;
using namespace std;

TCanvas *c1 = new TCanvas("c1","c1");
//c1->SetLogy(0);
//const double minZ_cm=ZMIN;
//const double maxZ_cm=ZMAX;
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
//const int nBinsZ=NZBIN;
const int nBinsDt=NDTBIN;
const int nBinsRadius=NRBIN;
const int nBinsPhi=NPHIBIN;
const int livedays=EXPOSURE;

TString basedir(DATABASEDIR);

//cout<<"Looking for input files in "<<basedir<<endl;

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
//Double_t pfcEfficiency=0.969; //result of hand scanning
Double_t pfcEfficiency=1.0; //Evan accounts for this in his efficiency
// 3% of waveforms weren't classified as golden when they were
Double_t simExposure = 7.*3400.; // did 4x 850 livedays (=3400), then did 7x on that
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

void makeTH3(RooWorkspace *bgws,const int popInd, const int time_bin)
{
  cout<<"Creating "<<labels[popInd]<<" in time bin "<<time_bin<<"..."<<endl;
  TDirectory *d=gDirectory;
  //TH3F hist("hist","hist",  nBinsRadius,0,maxRadius,  nBinsPhi,minPhi,maxPhi,  nBinsDt,minDt,maxDt);
  TH3F* hist = spatialBinDef();
  hist->SetName("hist");
  
  double evtCount = 0.;
  double counts = 0.;
  TArrayI comIds=componentIds[popInd];
   // Parse the spatial binning of the detector response model
  /*
   */
  for(int i=0;i<comIds.GetSize();i++)
  {
    cout<<"Opening "<<basedir+TString("golden")+filenames[comIds[i]]+TString::Format("_tb%d_",time_bin)+TString("fidCut.root")<<endl;
    TFile *f =TFile::Open(basedir+TString("golden")+filenames[comIds[i]]+TString::Format("_tb%d_",time_bin)+TString("fidCut.root"));
    d->cd();
    TTree* golden = (TTree*)f->Get("golden");
    
    golden->SetBranchAddress("r",&r);
    golden->SetBranchAddress("drift",&drift);
    golden->SetBranchAddress("phi",&phi);
    golden->SetBranchAddress("S1",&S1);
    golden->SetBranchAddress("log10S2",&log10S2);
    golden->SetBranchAddress("rawS2",&rawS2);
    Int_t nentries = (Int_t)golden->GetEntries();
    for(Int_t evt=0; evt<nentries; evt++){
       golden->GetEntry(evt);
       
       if( drift>DTMIN && drift<DTMAX && r<FIDRADIUS && phi>PHIMIN && phi<PHIMAX && rawS2>S2RAWTHRESHOLD && S1>S1MIN && S1<S1MAX
           && log10S2>LOG10S2MIN && log10S2<LOG10S2MAX && log10S2<ERCut_3sig(S1) && log10S2>NRCut_7sig(S1) ){
          hist->Fill( r, phi, drift, weights[comIds[i]]*EventEfficiency(rawS2));
          evtCount += weights[comIds[i]]*EventEfficiency(rawS2);
       }
       
    }
    cout<<"Bkg: "<<labels[popInd]<<"\t"<<"Filename: "<<filenames[comIds[i]]<<"\t"<<"Weight: "<<weights[comIds[i]]<<endl;
    //cout<<golden->Draw("r:phi:drift>>+hist",TString::Format("%f*%s",weights[comIds[i]], FV.GetTitle()),"colz")<<endl;
    f->Close();
  }
  
  cout<<"Opening output file..."<<endl;
  TFile *outfile = new TFile(TString::Format("./PDFs/Bkg_rphidrift_TH3F_%s_TB%d.root",labels[popInd].Data(),time_bin),"RECREATE");
  TH3F *spatial_hist = (TH3F*)hist->Clone("spatial_hist");
  TString timeString = TString::Format("tb%d",time_bin);
  TString name(labels[popInd]+"_rphidrift_"+timeString);
  TString title(labels[popInd]+"_rphidrift_"+timeString);
  spatial_hist->SetNameTitle(name,title);
  
  //Smooth
  counts = spatial_hist->Integral();
  Smooth3D( spatial_hist );
  
  cout<<"Saving Histogram..."<<endl;
  //spatial_hist->SaveAs( name+".C" );
  spatial_hist->Write();
  cout<<"Histogram saved"<<endl;
  // Now split up the full spatial PDF into separate PDFs for each z-slice (kept in general dimensionality for now)
  // First, declare a histogram for each z-slice
   TH3F* h_binDef = modelBinDef(time_bin);;
   const int nModelBinsX = (const int)h_binDef->GetNbinsX();
   const int nModelBinsY = (const int)h_binDef->GetNbinsY();
   const int nModelBinsZ = (const int)h_binDef->GetNbinsZ();
   //TH3F *hist_array[nModelBinsX][nModelBinsY][nModelBinsZ];
   TH3F *hist_array[1][1][4]; // because fuck ROOT
   for (int ix = 0; ix < nModelBinsX; ix++) {
      for (int iy = 0; iy < nModelBinsY; iy++) {
	 for (int iz = 0; iz < nModelBinsZ; iz++) {
	    // Naming convention: positionPDFcyl_TimeBin<timebin>__R<rbin>_PHI<phibin>_DT<dtbin>
	    // where each bin number goes from 1 to Nbins (inclusive)
	    TString histname = name + TString::Format("_R%d_PHI%d_DT%d",ix+1,iy+1,iz+1);
	    //positionPDFcyl_array[ix][iy][iz] = new TH3F(histname.Data(),histname.Data(),NRBIN,0,FIDRADIUS,NPHIBIN,PHIMIN,PHIMAX,NDTBIN,DTMIN,DTMAX);
	    hist_array[ix][iy][iz] = spatialBinDef();
	    hist_array[ix][iy][iz]->SetName(histname.Data());
	    hist_array[ix][iy][iz]->SetTitle(histname.Data());

	 }
      }
   }
   // Now loop through bins of the finished, full spatial PDF. Carefully copy the bin contents to each z-slice PDF
   const int nSpaceBinsX = spatial_hist->GetNbinsX();
   const int nSpaceBinsY = spatial_hist->GetNbinsY();
   const int nSpaceBinsZ = spatial_hist->GetNbinsZ();
   for (int ix = 0; ix < nSpaceBinsX; ix++) {
      double r = spatial_hist->GetXaxis()->GetBinCenter(ix+1);
      for (int iy = 0; iy < nSpaceBinsY; iy++) {
	 double phi = spatial_hist->GetYaxis()->GetBinCenter(iy+1);
	 for (int iz = 0; iz < nSpaceBinsZ; iz++) {
	    double drift = spatial_hist->GetZaxis()->GetBinCenter(iz+1);
	    double thisBinContent = spatial_hist->GetBinContent(ix+1,iy+1,iz+1);
	    // Use the bin center as the reference to determine z-slice
	    int rbin,phibin,dtbin;
	    int globalBin = h_binDef->FindBin(r,phi,drift);
	    h_binDef->GetBinXYZ(globalBin,rbin,phibin,dtbin);
	    if (rbin < 1) rbin = 1; 
	    if (rbin > h_binDef->GetNbinsX()) rbin = h_binDef->GetNbinsX();
	    if (phibin < 1) phibin = 1; 
	    if (phibin > h_binDef->GetNbinsY()) phibin = h_binDef->GetNbinsY();
	    if (dtbin < 1) dtbin = 1; 
	    if (dtbin > h_binDef->GetNbinsZ()) dtbin = h_binDef->GetNbinsZ();
	    // Fill the appropriate z-slice histogram
	    hist_array[rbin-1][phibin-1][dtbin-1]->SetBinContent(ix+1,iy+1,iz+1,thisBinContent);
	 }
      }
   }
   // Now, write to file! The name should already be correct.
   // And since these were sliced from the already-smoothed histogram, no smoothing necessary.
   for (int ix = 0; ix < nModelBinsX; ix++) {
      for (int iy = 0; iy < nModelBinsY; iy++) {
	 for (int iz = 0; iz < nModelBinsZ; iz++) {
	    hist_array[ix][iy][iz]->Write();
	 }
      }
   }
  
  bgws->factory(TString::Format( "%s%d_expectation[%f]",(char*)(labels[popInd]),time_bin,counts )); //expected number of counts
  bgws->var(TString::Format("%s%d_expectation",(char*)(labels[popInd]),time_bin))->Print();
  
  //cout<<"Expected From Integral:   "<< counts <<endl;
  //cout<<"Expected From Weight Sum: "<< evtCount <<endl;
  
  outfile->Close();
  cout<<"Done"<<endl;
}

void spatialTH3FromNtuples()
{
  RooWorkspace *bgws = new RooWorkspace("bgws");
  // loop over pdfs to be created
  // and loop over time bins
  // nine files get created: one for each time bin and pdf type
  for (int i=0;i<nPop;i++){ //nPop
    
    for (int j=0;j<NTIMEBIN;j++){ //NTIMEBIN
       makeTH3( bgws, i, j+1 );
    }
    
  }
  bgws->SaveAs("bgws.root");
  gApplication->Terminate();
}
