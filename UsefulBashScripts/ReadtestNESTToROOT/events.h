//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May 19 14:10:03 2018 by ROOT version 5.34/30
// from TTree events/LUX Fast Bkg Events Tree 8B
// found on file: result.root
//////////////////////////////////////////////////////////

#ifndef events_h
#define events_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class events {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           iEvtN;
   Float_t         x_cm;
   Float_t         y_cm;
   Float_t         r_cm;
   Float_t         z_cm;
   Float_t         phi;
   Float_t         dt_s2;
   Float_t         energy_keV;
   Float_t         s1_raw;
   Float_t         s1c;
   Float_t         s1s;
   Float_t         s2_raw;
   Float_t         s2c;
   Float_t         log10S2;
   Float_t         Efield;
   Float_t         DriftTime;

   // List of branches
   TBranch        *b_iEvtN;   //!
   TBranch        *b_x_cm;   //!
   TBranch        *b_y_cm;   //!
   TBranch        *b_r_cm;   //!
   TBranch        *b_z_cm;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_dt_s2;   //!
   TBranch        *b_energy_keV;   //!
   TBranch        *b_s1_raw;   //!
   TBranch        *b_s1c;   //!
   TBranch        *b_s1s;   //!
   TBranch        *b_s2_raw;   //!
   TBranch        *b_s2c;   //!
   TBranch        *b_log10S2;   //!
   TBranch        *b_Efield;   //!
   TBranch        *b_DriftTime;   //!

   events(TTree *tree=0);
   virtual ~events();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef events_cxx
events::events(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
 /*  if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("result.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("result.root");
      }
      f->GetObject("events",tree);

   }*/
   Init(tree);
}

events::~events()
{
//   if (!fChain) return;
//   delete fChain->GetCurrentFile();
}

Int_t events::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t events::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void events::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("iEvtN", &iEvtN, &b_iEvtN);
   fChain->SetBranchAddress("x_cm", &x_cm, &b_x_cm);
   fChain->SetBranchAddress("y_cm", &y_cm, &b_y_cm);
   fChain->SetBranchAddress("r_cm", &r_cm, &b_r_cm);
   fChain->SetBranchAddress("z_cm", &z_cm, &b_z_cm);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("dt_s2", &dt_s2, &b_dt_s2);
   fChain->SetBranchAddress("energy_keV", &energy_keV, &b_energy_keV);
   fChain->SetBranchAddress("s1_raw", &s1_raw, &b_s1_raw);
   fChain->SetBranchAddress("s1c", &s1c, &b_s1c);
   fChain->SetBranchAddress("s1s", &s1s, &b_s1s);
   fChain->SetBranchAddress("s2_raw", &s2_raw, &b_s2_raw);
   fChain->SetBranchAddress("s2c", &s2c, &b_s2c);
   fChain->SetBranchAddress("log10S2", &log10S2, &b_log10S2);
   fChain->SetBranchAddress("Efield", &Efield, &b_Efield);
   fChain->SetBranchAddress("DriftTime", &DriftTime, &b_DriftTime);
   Notify();
}

Bool_t events::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void events::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t events::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef events_cxx
