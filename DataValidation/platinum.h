//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 21 20:46:48 2018 by ROOT version 5.34/30
// from TTree platinum/Distilled TTree, for limits code
// found on file: eft_bg_data_trial_05182018.root
//////////////////////////////////////////////////////////

#ifndef platinum_h
#define platinum_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class platinum {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   ULong64_t       luxstamp;
   Float_t         log10S2;
   Float_t         S1;
   Float_t         rawS2;
   Float_t         r;
   Float_t         phi;
   Float_t         drift;
   Int_t           timeBin;

   // List of branches
   TBranch        *b_luxstamp;   //!
   TBranch        *b_log10S2;   //!
   TBranch        *b_S1;   //!
   TBranch        *b_rawS2;   //!
   TBranch        *b_r;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_drift;   //!
   TBranch        *b_timeBin;   //!

   platinum(TTree *tree=0);
   virtual ~platinum();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef platinum_cxx
platinum::platinum(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("eft_bg_data_trial_05182018.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("eft_bg_data_trial_05182018.root");
      }
      f->GetObject("platinum",tree);

   }
   Init(tree);
}

platinum::~platinum()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t platinum::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t platinum::LoadTree(Long64_t entry)
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

void platinum::Init(TTree *tree)
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

   fChain->SetBranchAddress("luxstamp", &luxstamp, &b_luxstamp);
   fChain->SetBranchAddress("log10S2", &log10S2, &b_log10S2);
   fChain->SetBranchAddress("S1", &S1, &b_S1);
   fChain->SetBranchAddress("rawS2", &rawS2, &b_rawS2);
   fChain->SetBranchAddress("r", &r, &b_r);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("drift", &drift, &b_drift);
   fChain->SetBranchAddress("timeBin", &timeBin, &b_timeBin);
   Notify();
}

Bool_t platinum::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void platinum::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t platinum::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef platinum_cxx
