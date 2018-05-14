//Macro to generate RooHistPdfs in log10S2 v S1 and z_cm v rSquared
// 5 histograms (S1,S2 and r,z taken to be independent).
// extrinsic (S1,S2), (r,z)
// intrinsic xrays (S1,S2),(r,z)
// intrinsic betas (S1,S2)
// ======> EVENT:0
//  s1              = 1
//  s2              = 2
//  luxstamp        = 12019986000000000
//  s1area_raw      = 26.969
//  s1area          = 27.8098
//  s2area_raw      = 1712.85
//  s2area          = 2173.32
//  goodArea        = 1739.82
//  badArea         = 0.543457
//  spikyS1         = 25.132
//  spikyS1_raw     = 22
//  spikyS2         = 1818.19
//  spikyS2_raw     = 1460
//  drift           = 142.89
//  zdrift          = 143.28
//  corrected_x     = 0
//  corrected_y     = 0
//  cm_x            = 12.9176
//  cm_y            = 4.88167
//  correctedRadius = 13.8092
//  elife_correction = 1.23975
//  s1xyz_correction = 1.03118
//  s2xy_correction = 1.00451
//  q2              = 30.4106
//  dof             = 24
//  sd_Rsup         = 0.374516
//  sd_Rinf         = 0.362434
//  sdPhi           = 0.374516
//
// 20150220 v2 
//   - include top array, bottom array, and other external gammas from separate files
//   - smooth the TH2s before making the RooHistPdfs with TH2::Smooth(1,"k5b")
//   - add a consolidated S1-S2 pdf for all the ~flat-in-E ER backgrounds
//
// 20150514 v3
//   - The script that makes these ntuples had a couple of misapplied corrections in the calculation of s2area, so divide them out with 2area*=(0.53/0.54/s2xy_correction)
//   - Added high-stats flat-in-E ER as an pdf -- smooth general-purpose ER
//
// 20150606 v4
//   - including 20150605 LUXSim flat ER (cp14847) tuned to tritium band by KO
//   - increased Xe-127 weighting by ratio of DH's 5-keV branching fraction to the G4 value
//   - yields of Xe-127 adjusted, post-LUXSim in the hist-filling step, by 10% (increased charge yield to match DH's measurement, decreased light yield to conserve quanta)

//#include "RooDataSet.h"


#include "RooNDKeysPdf.h"

#include "../Parameters.h"


using namespace RooFit;

TCanvas *c1 = new TCanvas("c1","c1");
c1->SetLogy(0);
//const double minZ_cm=ZMIN;
//const double maxZ_cm=ZMAX;
const double minDt_us=DTMIN;
const double maxDt_us=DTMAX;
const double maxRadius_cm=FIDRADIUS;
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

//TCut FV(TString::Format("(z_cm>%f&&z_cm<%f&&r_cm<%f&&phi>%f&&phi<%f&&s2area_raw>%f&&spikyS1>%f&&spikyS1<%f)",
//                        minZ_cm,maxZ_cm,maxRadius_cm,minPhi,maxPhi,minRawS2_phe,minS1,maxS1));

//TCut FV(TString::Format("(z>%f&&z<%f&&r<%f&&phi>%f&&phi<%f&&rawS2>%f&&S1>%f&&S1<%f)",
 //                       minZ_cm,maxZ_cm,maxRadius_cm,minPhi,maxPhi,minRawS2_phe,minS1,maxS1));
TCut FV(TString::Format("(drift>%f&&drift<%f&&r<%f&&phi>%f&&phi<%f&&rawS2>%f&&S1>%f&&S1<%f)",
                        minDt_us,maxDt_us,maxRadius_cm,minPhi,maxPhi,minRawS2_phe,minS1,maxS1));

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
"Ex_All_fidCut",
"FCS_Co60_fidCut",
"FCS_Ra226_fidCut",
"FCS_Tl208_fidCut",
"PMTB_All_fidCut",
"PMTT_All_fidCut",
"Xe_Kr85_fidCut",
"Xe_Rn220_fidCut",
"Xe_Rn222_fidCut",
"Xe_Xe127_fidCut"
};
Double_t pfcEfficiency=0.969; //result of hand scanning
// 3% of waveforms weren't classified as golden when they were
Double_t simExposure = 80.*340.;
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

/*
const int nPop=6;
const int nc[nPop]={1,3,4,2,6,9}; //number inputs for each pdf
TArrayI componentIds[nPop]; // 
TString labels[nPop];  //name of pdf

labels[0]="Xe";
int xeComA[nc[0]]={9}; //"xenon componet array", position in the filenames array
TArrayI xeCom(nc[0],xeComA); //array of arrays of ints, number of elements and a regular c++ array
componentIds[0]=xeCom; //componentIds = array of arrays. first index = which pdf, second index = position in filenames array

labels[1]="RnKr";
int betaComA[nc[1]]={6,7,8};
TArrayI betaCom(nc[1],betaComA);
componentIds[1]=betaCom;

labels[2]="ComptonBottom";
int comptonBottomComA[nc[2]]={1,2,3,4};
TArrayI comptonBottomCom(nc[2],comptonBottomComA);
componentIds[2]=comptonBottomCom;

labels[3]="ComptonRest";
int comptonRestComA[nc[3]]={0,5};
TArrayI comptonRestCom(nc[3],comptonRestComA);
componentIds[3]=comptonRestCom;

labels[4]="ComptonAll";
int comptonAllComA[nc[4]]={0,1,2,3,4,5};
TArrayI comptonAllCom(nc[4],comptonAllComA);
componentIds[4]=comptonAllCom;

labels[5]="consolidatedEr";
int consolidatedErComA[nc[5]]={0,1,2,3,4,5,6,7,8};
TArrayI consolidatedErCom(nc[5],consolidatedErComA);
componentIds[5]=consolidatedErCom;
*/
void addRooHistPdf_s1ls2(RooWorkspace *bgws, const int popInd)
{
  cout<<"Getting expected num events"<<endl;
  TDirectory *d=gDirectory;
  RooArgSet ras_s1ls2=bgws->argSet("S1,log10S2");
  TH2D s1ls2hist("s1ls2hist","s1ls2hist",nBinsS1,minS1,maxS1,nBinslog10S2,minlog10S2,maxlog10S2);
  TArrayI comIds=componentIds[popInd];
  for(int i=0;i<comIds.GetSize();i++)
  {
    TFile *f =TFile::Open(basedir+TString("golden")+filenames[comIds[i]]+TString(".root"));
    d->cd();
    TTree*golden=f->Get("platinum");
    //cout<<labels[popInd]<<"\t"<<filenames[comIds[i]]<<"\t"<<weights[comIds[i]]<<endl;
    cout<<golden->Draw("log10S2:S1>>+s1ls2hist",TString::Format("%f*%s",weights[comIds[i]], FV.GetTitle()),"colz")<<endl;
    f->Close();
  }
  //s1ls2hist.Smooth(1,smoothOpt); // smoothing
  s1ls2hist.SetTitle(labels[popInd]);
  //c1->Print(labels[popInd]+"_s1ls2.pdf");
  //s1ls2hist.SaveAs(labels[popInd]+"_s1ls2.root");
  TString name(labels[popInd]+"_s1ls2");
  TString title(labels[popInd]+"_s1ls2");
  //RooDataHist rdh1(name,title,RooArgList(ras_s1ls2),&s1ls2hist);
  //RooHistPdf rhp1(name,title,ras_s1ls2,rdh1);
  //bgws->import(rhp1);
  bgws->factory(TString::Format("%s_expectation[%f]",(char*)(labels[popInd]),s1ls2hist.Integral())); //expected number of counts
  bgws->var(TString::Format("%s_expectation",(char*)(labels[popInd])))->Print();
  
  cout<<"From s1s2 hist integral: "<< s1ls2hist.Integral() << endl;
  cout<<"Done getting expected num events-------------------"<<endl;
  
}

void addRooHistPdf(RooWorkspace *bgws, const int popInd, const int time_bin)
{
  TDirectory *d=gDirectory;
  RooArgSet ras_all = bgws->argSet("r,drift,phi,S1,log10S2,timeBin");
  RooArgSet ras_5vars = bgws->argSet("r,drift,phi,S1,log10S2");
  //TH2D rzhist("rzphihist","rzphihist",nBinsRadius,0,maxRadius_cm,nBinsZ,minZ_cm,maxZ_cm);
  //TH3D rzphihist("rzphihist","rzphihist",nBinsRadius,0,maxRadius_cm,nBinsZ,minZ_cm,maxZ_cm,nBinsPhi,minPhi,maxPhi);
  TArrayI comIds=componentIds[popInd];
  
  //const int numComs = comIds.GetSize();
  //RooDataSet* rds_array[numComs];
  
  //make hist container
  RooDataHist rdh("rdh","rdh",ras_5vars);
  
  //RooRealVar weight("weight","weight",0,1.);
  RooFormulaVar* var = 0; //dummy var
  
  for(int i=0;i<comIds.GetSize();i++){
    
    //Open file, get tree, put it in data set
    TFile *f =TFile::Open(basedir+TString("golden")+filenames[comIds[i]]+TString(".root"));
    d->cd(); TTree*golden=f->Get("platinum");
    RooDataSet* rds = new RooDataSet( "rds","rds",ras_all,Import(*golden),Cut(TString::Format("timeBin==%d",time_bin)) ); //cuts go here!!
    
    //bgws->var("weight")->setVal( weights[comIds[i]] ); //set weight for this dataset
    
    //cout<<"importing " << filenames[comIds[i]] << "dataset"<<endl;
    //rds_array[i] = new RooDataSet("rds_data","rds_data",ras_all,WeightVar("weight"),Import(*golden));
    //cout<<"numEntries() : "<< rds_array[i]->numEntries() <<endl;
    //cout<<"sumEntries() : "<< rds_array[i]->sumEntries() <<endl;
    //cout<<endl;
    
    cout<<"Adding to pdf "<<labels[popInd]<<" from file "<<filenames[comIds[i]]<<" with weight "<<weights[comIds[i]]<<" in time bin "<<time_bin<<endl;
    cout<<"RooDataSet numEntries: "<< rds->numEntries() <<endl;
    
    //Add data set to hist with correct weight
    rdh.add( *rds, var, weights[comIds[i]] );
    
    //golden->Draw("r:z:phi>>+rzphihist",TString::Format("%f*%s",weights[comIds[i]], FV.GetTitle()));
    f->Close();
    //cout<<labels[popInd]<<"\t"<<filenames[comIds[i]]<<"\t"<<weights[comIds[i]]<<endl;
    
    delete rds;
  }
  //cout<<"RooDataHist sumEntries: "<< rdh.sumEntries() <<endl;
  cout<<"Making PDF out of RooDataHist with numEntries = "<< rdh.numEntries() <<endl;
  //make it into a pdf
  TString timeString = TString::Format("%d",time_bin);
  TString name(labels[popInd]+"_5d_"+timeString);
  TString title(labels[popInd]+"_5d_"+timeString);
  RooHistPdf rhp(name,title,ras_5vars,rdh);
  bgws->import(rhp);
  cout<<endl;
  //cout<<"Expected Num "<< labels[popInd] <<" Events = "<< rdh.sumEntries() <<endl;
  //bgws->factory(TString::Format("%s_expectation[%f]",(char*)(labels[popInd]),rdh.sumEntries())); //expected number of counts
  //bgws->var(TString::Format("%s_expectation",(char*)(labels[popInd])))->Print();
  
  bool checkPDFval = false;
  if( checkPDFval && time_bin==1 && labels[popInd].EqualTo("ComptonRest")){
     
     TH2D hist_s1s2("s1s2","s1s2",nBinsS1,minS1,maxS1,nBinslog10S2,minlog10S2,maxlog10S2);
     TH2D hist_rz("rz","rz",NRBIN,0,FIDRADIUS,NDTBIN,DTMIN,DTMAX);
     Double_t S1_binCenter;
     Double_t S2_binCenter;
     Double_t r_binCenter;
     Double_t drift_binCenter;
     Double_t phi_binCenter;
     Double_t pdfVal;
     int count = 0;
     
     cout<<"Looping over PDF bins..."<<endl;
     for(int t=0; t<NLOGS2BIN; t++){
          for(int m=0; m<NRBIN; m++){
             for(int n=0; n<NDTBIN; n++){
               for(int k=0; k<NPHIBIN; k++){
                 for(int ii=0; ii<NS1BIN; ii++){
                
                  //get value of bin center
                  S1_binCenter = bgws->var("S1")->getBinning().binCenter( ii );
                  S2_binCenter = bgws->var("log10S2")->getBinning().binCenter( t );
                  r_binCenter = bgws->var("r")->getBinning().binCenter( m );
                  drift_binCenter = bgws->var("drift")->getBinning().binCenter( n );
                  phi_binCenter = bgws->var("phi")->getBinning().binCenter( k );
                  //cout<<"Bin "<< i <<" center at "<< S1_binCenter <<endl;
                  //set value of variable to bin center value
                  bgws->var("S1")->setVal( S1_binCenter );
                  bgws->var("log10S2")->setVal( S2_binCenter );
                  bgws->var("r")->setVal( r_binCenter );
                  bgws->var("drift")->setVal( drift_binCenter );
                  bgws->var("phi")->setVal( phi_binCenter );
                  //extract value of pdf at bin center
                  pdfVal = rhp.getVal( ras_5vars );
                  
                  if( pdfVal > 0 ){
                     
                     hist_s1s2->SetBinContent( ii, t, pdfVal*1.e6);
                     hist_rz->SetBinContent( m, n, pdfVal*1.e6);
                     
                     cout<<"S1 bin center      "<<ii<<endl;
                     cout<<"log10S2 bin center "<<t<<endl;
                     cout<<"r bin center       "<<m<<endl;
                     cout<<"drift bin center   "<<n<<endl;
                     cout<<"phi bin center     "<<k<<endl;
                     cout<<"pdf Value center   "<<pdfVal<<endl;
                     cout<<endl;
                     count++;
                  }
                 //put it in the RooDataHist
                 //rdh.add( ras_5vars, pdfVal*weights[comIds[i]] );
                 }
               }
             }
          }
       }
     cout<<"There are "<<count<<" bins with non-zero pdf value"<<endl;
     //hist->Draw("colz");
     hist_s1s2->SaveAs("pdfVals_s1s2.root");
     hist_rz->SaveAs("pdfVals_rz.root");
  } //if
  
}

void addRooHistPdfFromKeys(RooWorkspace *bgws, const int popInd)
{
  TDirectory *d=gDirectory;
  //RooArgSet ras_all = bgws->argSet("r,z,phi,S1,log10S2,weight");
  RooArgSet ras_5vars = bgws->argSet("r,drift,phi,S1,log10S2");
  //TH2D rzhist("rzphihist","rzphihist",nBinsRadius,0,maxRadius_cm,nBinsZ,minZ_cm,maxZ_cm);
  //TH3D rzphihist("rzphihist","rzphihist",nBinsRadius,0,maxRadius_cm,nBinsZ,minZ_cm,maxZ_cm,nBinsPhi,minPhi,maxPhi);
  TArrayI comIds=componentIds[popInd];
  
  //const int numComs = comIds.GetSize();
  //cout<<"Number of input files to grab: "<<numComs<<endl;
  //RooDataSet* rds_array[numComs];
  
  //make hist container
  RooDataHist rdh("rdh","rdh",ras_5vars);
  
  //RooRealVar weight("weight","weight",0,1.);
  RooFormulaVar* var = 0; //dummy var
  
  Double_t S1_binCenter;
  Double_t S2_binCenter;
  Double_t r_binCenter;
  Double_t drift_binCenter;
  Double_t phi_binCenter;
  Double_t pdfVal;
  
  for(int i=0;i<comIds.GetSize();i++){
    
    //Open file, get tree, put it in data set
    TFile *f =TFile::Open(basedir+TString("golden")+filenames[comIds[i]]+TString(".root"));
    d->cd(); TTree*golden=f->Get("platinum");
    RooDataSet* rds = new RooDataSet("rds","rds",ras_5vars,Import(*golden)); //cuts go here!!
    cout<<"Input dataset has "<< rds->numEntries() <<" entries"<<endl;
    if( rds->numEntries() < 1 || filenames[comIds[i]] == "Xe_Rn220_fidCut" ) continue;
    
    //bgws->var("weight")->setVal( weights[comIds[i]] ); //set weight for this dataset
    
    //cout<<"importing " << filenames[comIds[i]] << "dataset"<<endl;
    //rds_array[i] = new RooDataSet("rds_data","rds_data",ras_all,WeightVar("weight"),Import(*golden));
    //cout<<"numEntries() : "<< rds_array[i]->numEntries() <<endl;
    //cout<<"sumEntries() : "<< rds_array[i]->sumEntries() <<endl;
    //cout<<endl;
    
    cout<<labels[popInd]<<"\t"<<filenames[comIds[i]]<<"\t"<<weights[comIds[i]]<<endl;
    
    //Add data set to hist with correct weight
    //rdh.add( *rds, var, weights[comIds[i]] );
    
    RooNDKeysPdf rkp("rkp", "rkp", ras_5vars, *rds,"m");
    cout<<"keys pdf has been made"<<endl;
    
    //loop over bin centers and get value of keysPdf:
    //RooDataHist rdh("rdh","rdh",ras_5vars); //an empty RooDataHist
   
   //S1_binCenter = bgws->var("S1")->getBinning().binCenter( 1 );
   //cout<<bgws->var("S1")->getBinning().numBins()<<endl;
   //cout<<S1_binCenter<<endl;
   
   // for(Int_t ii=0; i<NS1BIN; i++){
       for(int j=0; j<NLOGS2BIN; j++){
          for(int m=0; m<NRBIN; m++){
             for(int n=0; n<NDTBIN; n++){
               for(int k=0; k<NPHIBIN; k++){
                 for(int ii=0; ii<NS1BIN; ii++){
                
                  //get value of bin center
                  S1_binCenter = bgws->var("S1")->getBinning().binCenter( ii );
                  S2_binCenter = bgws->var("log10S2")->getBinning().binCenter( j );
                  r_binCenter = bgws->var("r")->getBinning().binCenter( m );
                  drift_binCenter = bgws->var("drift")->getBinning().binCenter( n );
                  phi_binCenter = bgws->var("phi")->getBinning().binCenter( k );
                  //cout<<"Bin "<< i <<" center at "<< S1_binCenter <<endl;
                  //set value of variable to bin center value
                  bgws->var("S1")->setVal( S1_binCenter );
                  bgws->var("log10S2")->setVal( S2_binCenter );
                  bgws->var("r")->setVal( r_binCenter );
                  bgws->var("drift")->setVal( drift_binCenter );
                  bgws->var("phi")->setVal( phi_binCenter );
                  //extract value of pdf at bin center
                  pdfVal = rkp.getVal( ras_5vars );
                  //cout<<"pdfVal: "<< pdfVal <<endl;
                  
                  /*cout<<"S1 bin "<<ii<<endl;
                  cout<<"log10S2 bin "<<j<<endl;
                  cout<<"r bin "<<m<<endl;
                  cout<<"drift bin "<<n<<endl;
                  cout<<"phi bin "<<k<<endl;
                  cout<<"pdf Value "<<pdfVal<<endl;
                  cout<<endl;*/
                  
                 //put it in the RooDataHist
                 rdh.add( ras_5vars, pdfVal*weights[comIds[i]] );
                 }
               }
             }
          }
        cout<<"log10S2 bin "<<j<<endl;
       }
    //}

    cout<<"Done filling RooDataHist!"<<endl;
    
    //golden->Draw("r:z:phi>>+rzphihist",TString::Format("%f*%s",weights[comIds[i]], FV.GetTitle()));
    f->Close();
    //cout<<labels[popInd]<<"\t"<<filenames[comIds[i]]<<"\t"<<weights[comIds[i]]<<endl;
    
    delete rds;
    //delete rkp;
  }
  
  //cout<<"RooDataHist sumEntries: "<< rdh.sumEntries() <<endl;
  //cout<<"RooDataHist numEntries: "<< rdh.numEntries() <<endl;
  //make it into a pdf
  TString name(labels[popInd]+"_5d");
  TString title(labels[popInd]+"_5d");
  RooHistPdf rhp(name,title,ras_5vars,rdh);
  bgws->import(rhp);
  
  //cout<<"Expected Num "<< labels[popInd] <<" Events = "<< rdh.sumEntries() <<endl;
  //bgws->factory(TString::Format("%s_expectation[%f]",(char*)(labels[popInd]),rdh.sumEntries())); //expected number of counts
  //bgws->var(TString::Format("%s_expectation",(char*)(labels[popInd])))->Print();
  
}

void addKeysPdf(RooWorkspace *bgws, const int popInd)
{
  TDirectory *d=gDirectory;
  //RooArgSet ras_5vars = bgws->argSet("r,z,phi,S1,log10S2");
  RooArgSet ras_5vars = bgws->argSet("r,z,phi,S1,log10S2,weight");
  TArrayI comIds=componentIds[popInd];
  
  //Float_t z_in, r_in, phi_in, S1_in, log10S2_in;
  //RooDataSet* rds_array[numComs];
  RooFormulaVar* var = 0; //dummy var
  
  RooDataSet* rds = new RooDataSet("rds","rds",ras_5vars, WeightVar("weight"));
  
  for(int i=0;i<comIds.GetSize();i++){
    
    //Open file, get tree, put it in data set
    TFile *f =TFile::Open(basedir+TString("golden")+filenames[comIds[i]]+TString(".root"));
    d->cd(); TTree*golden=f->Get("platinum");
    /*
    //Set branches to local variables:
    golden->SetBranchAddress("z", &z_in);
    golden->SetBranchAddress("r", &r_in);
    golden->SetBranchAddress("phi", &phi_in);
    golden->SetBranchAddress("S1", &S1_in);
    golden->SetBranchAddress("log10S2", &log10S2_in);
    */
    //RooDataSet* rds = new RooDataSet("rds","rds",ras_5vars,Import(*golden)); //cuts go here!!
    
    bgws->var("weight")->setVal( weights[comIds[i]] ); //set weight for this dataset
    cout<<"importing " << filenames[comIds[i]] << "dataset"<<endl;
    RooDataSet* rds_tmp = new RooDataSet("rds_tmp","rds_tmp",ras_5vars,WeightVar("weight"),Import(*golden));
    cout<<"numEntries() : "<< rds_tmp->numEntries() <<endl;
    cout<<"sumEntries() : "<< rds_tmp->sumEntries() <<endl;
    cout<<endl;
    
    //append tmp data set to big data set
    rds->append( *rds_tmp );
    
    /*
    //Loop over golden tree evts and put them into rooDataSet
    //with weights
    for(int ii=0;golden->GetEntries();ii++){
       golden->GetEntry(ii);
       bgws->var("S1")->setVal( S1_in );
       bgws->var("log10S2")->setVal( log10S2_in );
       bgws->var("r")->setVal( r_in );
       bgws->var("z")->setVal( z_in );
       bgws->var("phi")->setVal( phi_in );
       bgws->var("weight")->setVal( weights[comIds[i]] );
       
       //rds->add( ras_5vars, weights[comIds[i]] );
       rds->add( ras_5vars );
    }
    */
    cout<<labels[popInd]<<"\t"<<filenames[comIds[i]]<<"\t"<<weights[comIds[i]]<<endl;
    f->Close();
    
    //delete rds_tmp;
    //delete rkp;
  }
  
  //cout<<"RooDataHist sumEntries: "<< rdh.sumEntries() <<endl;
  //cout<<"RooDataHist numEntries: "<< rdh.numEntries() <<endl;
  //make it into a pdf
  TString name(labels[popInd]+"_5d");
  TString title(labels[popInd]+"_5d");
  //RooHistPdf rhp(name,title,ras_5vars,rdh);
  cout<<"making pdf..."<<endl;
  RooNDKeysPdf rkp("rkp", "rkp", bgws->argSet("r,z,phi,S1,log10S2"), *rds,"m");
  cout<<"keys pdf has been made"<<endl;
  RooCachedPdf rcp("rcp", "rcp", rkp, bgws->argSet("r,z,phi,S1,log10S2"));
  
  RooDataHist rdh("rdh","rdh",ras_5vars); //an empty RooDataHist
  
  for(int j=0; j<NLOGS2BIN; j++){
    for(int m=0; m<NRBIN; m++){
      for(int n=0; n<NZBIN; n++){
        for(int k=0; k<NPHIBIN; k++){
          for(int ii=0; ii<NS1BIN; ii++){
                
            //get value of bin center
            S1_binCenter = bgws->var("S1")->getBinning().binCenter( ii );
            S2_binCenter = bgws->var("log10S2")->getBinning().binCenter( j );
            r_binCenter = bgws->var("r")->getBinning().binCenter( m );
            z_binCenter = bgws->var("z")->getBinning().binCenter( n );
            phi_binCenter = bgws->var("phi")->getBinning().binCenter( k );
            //cout<<"Bin "<< i <<" center at "<< S1_binCenter <<endl;
            //set value of variable to bin center value
            bgws->var("S1")->setVal( S1_binCenter );
            bgws->var("log10S2")->setVal( S2_binCenter );
            bgws->var("r")->setVal( r_binCenter );
            bgws->var("z")->setVal( z_binCenter );
            bgws->var("phi")->setVal( phi_binCenter );
            //extract value of pdf at bin center
            pdfVal = rcp.getVal( ras_5vars );
            //cout<<"pdfVal: "<< pdfVal <<endl;
                  
            /*cout<<"S1 bin "<<ii<<endl;
            cout<<"log10S2 bin "<<j<<endl;
            cout<<"r bin "<<m<<endl;
            cout<<"z bin "<<n<<endl;
            cout<<"phi bin "<<k<<endl;
            cout<<"pdf Value "<<pdfVal<<endl;
            cout<<endl;*/
            
            //put it in the RooDataHist
            rdh.add( ras_5vars );
          }
        }
      }
    }
        cout<<"log10S2 bin "<<j<<endl;
  }
  
  RooHistPdf rhp(name,title,ras_5vars,rdh);
  
  bgws->import(rhp);
  
  cout<<"Expected Num "<< labels[popInd] <<" Events = "<< rds.sumEntries() <<endl;
  //bgws->factory(TString::Format("%s_expectation[%f]",(char*)(labels[popInd]),rdh.sumEntries())); //expected number of counts
  //bgws->var(TString::Format("%s_expectation",(char*)(labels[popInd])))->Print();
  
}

void rhpFromNtuples_5d_planA()
{
  RooWorkspace *bgws = new RooWorkspace("bgws");
  bgws->factory(TString::Format("r[0,%f]",maxRadius_cm));
  //bgws->factory(TString::Format("z[%f,%f]",minZ_cm,maxZ_cm));
  bgws->factory(TString::Format("drift[%f,%f]",minDt_us,maxDt_us));
  bgws->factory(TString::Format("phi[%f,%f]",minPhi,maxPhi));
  bgws->factory(TString::Format("S1[%f,%f]",minS1,maxS1));
  bgws->factory(TString::Format("log10S2[%f,%f]",minlog10S2,maxlog10S2));
  bgws->factory("weight[0,1.]");
  
  RooCategory timeBin( "timeBin", "Time Bin Category" );
  timeBin.defineType( "timeBin1", 1 );
  timeBin.defineType( "timeBin2", 2 );
  timeBin.defineType( "timeBin3", 3 );
  bgws->import(timeBin);
  
  //set binning for our variables
  RooBinning rbinning( NRBIN, 0., FIDRADIUS );
  //RooBinning zbinning( NZBIN, ZMIN, ZMAX );
  RooBinning driftbinning( NDTBIN, DTMIN, DTMAX );
  RooBinning phibinning( NPHIBIN, PHIMIN, PHIMAX );
  RooBinning S1binning( NS1BIN, S1MIN, S1MAX );
  RooBinning log10S2binning( NLOGS2BIN, LOG10S2MIN, LOG10S2MAX );
  
  bgws->var("r")->setBinning( rbinning );
  //bgws->var("z")->setBinning( zbinning );
  bgws->var("drift")->setBinning( driftbinning );
  bgws->var("phi")->setBinning( phibinning );
  bgws->var("S1")->setBinning( S1binning );
  bgws->var("log10S2")->setBinning( log10S2binning );
  
  //====================================================== 
  
  // loop over pdfs to be created
  // and loop over time bins
  for (int i=0;i<nPop;i++){
    
    for (int j=0;j<NTIMEBIN;j++){
       addRooHistPdf(bgws,i,j+1);
    }
    addRooHistPdf_s1ls2(bgws,i); //run this just to get expected # of counts
    cout<<endl; cout<<endl;
  }
  
  //int i = 1; //i=0 for RnKr, i=1 for ComptonBottom, i=2 for ComptonRest
  //addRooHistPdf(bgws,i);
  //addRooHistPdf_s1ls2(bgws,i); //run this just to get expected # of counts
  
  //addKeysPdf(bgws,0);
  //addRooHistPdfFromKeys(bgws,4);
  
  //addRooHistPdf_s1ls2(bgws,i);
  
  //addRooHistPdf_flatEr(bgws);
  //addRooHistPdf_Ar37(bgws);
  bgws->SaveAs("bgws.root");
  //if(i==0) bgws->SaveAs("bgws_RnKr1.root");
  //if(i==1) bgws->SaveAs("bgws_ComptonBottom1.root");
  //if(i==2) bgws->SaveAs("bgws_ComptonRest1.root");
  gApplication->Terminate();
}
