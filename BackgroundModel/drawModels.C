#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "modelBinDef.h"

void drawModels(int TimeBinNum=1) {
   // Open input file
   TFile *f = new TFile(TString::Format("Bkg_S1Log10S2_TH2F_TB%d.root",TimeBinNum),"OPEN");
   // Define spatial binning of response model
   TH3F* h_binDef = modelBinDef();
   const int nModelBinsX = h_binDef->GetNbinsX();
   const int nModelBinsY = h_binDef->GetNbinsY();
   const int nModelBinsZ = h_binDef->GetNbinsZ();
   TH2F* h_S1vslog10S2_array[nModelBinsX][nModelBinsY][nModelBinsZ]; // declare histo for each voxel
   TH2F* h_R2vsDT_S2_array[nModelBinsX][nModelBinsY][nModelBinsZ]; // declare histo for each voxel
   TH2F* h_XvsY_S2_array[nModelBinsX][nModelBinsY][nModelBinsZ]; // declare histo for each voxel
   TH1F* h_Efield_array[nModelBinsX][nModelBinsY][nModelBinsZ]; // declare histo for each voxel
   TCanvas* cans[nModelBinsX][nModelBinsY][nModelBinsZ];
   // Loop over model bins, make a canvas of plots for each one
   for (int ix = 0; ix < nModelBinsX; ix++) {
      for (int iy = 0; iy < nModelBinsY; iy++) {
	 for (int iz = 0; iz < nModelBinsZ; iz++) {
	    // Naming convention: h_S1vslog10S2_TimeBin<timebin>__R<rbin>_PHI<phibin>_DT<dtbin>
	    // where each bin number goes from 1 to Nbins (inclusive)
	    TString histname = TString::Format("h_S1vslog10S2_TimeBin%d__R%d_PHI%d_DT%d",TimeBinNum,ix+1,iy+1,iz+1);
	    h_S1vslog10S2_array[ix][iy][iz] = (TH2F*)f->Get(histname.Data());
	    cout << "Declared histogram " << histname.Data() << endl;
	    histname = TString::Format("h_R2vsDT_S2_TimeBin%d__R%d_PHI%d_DT%d",TimeBinNum,ix+1,iy+1,iz+1);
	    h_R2vsDT_S2_array[ix][iy][iz] = (TH2F*)f->Get(histname.Data());
	    histname = TString::Format("h_XvsY_S2_TimeBin%d__R%d_PHI%d_DT%d",TimeBinNum,ix+1,iy+1,iz+1);
	    h_XvsY_S2_array[ix][iy][iz] = (TH2F*)f->Get(histname.Data());
	    histname = TString::Format("h_Efield_TimeBin%d__R%d_PHI%d_DT%d",TimeBinNum,ix+1,iy+1,iz+1);
	    h_Efield_array[ix][iy][iz] = (TH1F*)f->Get(histname.Data());
	    // Begin drawing plots
	    TString canName = TString::Format("can_TimeBin%d__R%d_PHI%d_DT%d",TimeBinNum,ix+1,iy+1,iz+1);
	    cans[ix][iy][iz] = new TCanvas(canName.Data(),canName.Data(),800,800);
	    cans[ix][iy][iz]->Divide(2,2);
	    // ER Band (top left, hide stats box for remaining plots)
	    cans[ix][iy][iz]->cd(1);
	    h_S1vslog10S2_array[ix][iy][iz]->SetTitle("ER Band");
	    h_S1vslog10S2_array[ix][iy][iz]->SetXTitle("S1 [phd]");
	    h_S1vslog10S2_array[ix][iy][iz]->SetYTitle("Log_{10}(S2 [phd])");
	    h_S1vslog10S2_array[ix][iy][iz]->SetStats(0);
	    h_S1vslog10S2_array[ix][iy][iz]->Draw("col");
	    // Make and draw TProfile to summarize band mean and width
	    TProfile *profile = h_S1vslog10S2_array[ix][iy][iz]->ProfileX("_pfx",1,-1,"so");
	    profile->SetLineWidth(2);
	    profile->Draw("same");
	    // Electric field distribution (top right, show stats box)
	    cans[ix][iy][iz]->cd(2);
	    h_Efield_array[ix][iy][iz]->SetTitle("Electric Field Distribution");
	    h_Efield_array[ix][iy][iz]->SetXTitle("|E| [V/cm]");
	    h_Efield_array[ix][iy][iz]->SetStats(1);
	    h_Efield_array[ix][iy][iz]->Draw();
	    // Spatial distributions
	    // R^2 vs drift (bottom left)
	    cans[ix][iy][iz]->cd(3);
	    h_R2vsDT_S2_array[ix][iy][iz]->SetTitle(TString::Format("rBin=%d, phiBin=%d, dtBin=%d",ix+1,iy+1,iz+1));
	    h_R2vsDT_S2_array[ix][iy][iz]->SetXTitle("r_{S2}^{2} [cm^{2}]");
	    h_R2vsDT_S2_array[ix][iy][iz]->SetYTitle("drift [#mus]");
	    h_R2vsDT_S2_array[ix][iy][iz]->SetStats(0);
	    h_R2vsDT_S2_array[ix][iy][iz]->Draw("col");
	    // X vs Y (bottom right)
	    cans[ix][iy][iz]->cd(4);
	    h_XvsY_S2_array[ix][iy][iz]->SetTitle(TString::Format("rBin=%d, phiBin=%d, dtBin=%d",ix+1,iy+1,iz+1));
	    h_XvsY_S2_array[ix][iy][iz]->SetXTitle("x_{S2} [cm]");
	    h_XvsY_S2_array[ix][iy][iz]->SetYTitle("y_{S2} [cm]");
	    h_XvsY_S2_array[ix][iy][iz]->SetStats(0);
	    h_XvsY_S2_array[ix][iy][iz]->Draw("col");
	    //
	    cans[ix][iy][iz]->SaveAs(TString::Format("Bplus_%s.pdf",canName.Data()));
	    if (ix+iy+iz == (nModelBinsX-1)+(nModelBinsY-1)+(nModelBinsZ-1) ) {
	       //cans[ix][iy][iz]->SaveAs("myCoolGif.gif++50++");
	    } else {
	       //cans[ix][iy][iz]->SaveAs("myCoolGif.gif++50");
	    }
	 }
      }
   }
   /*TCanvas *c;
   gSystem->Unlink("myCoolGif.gif");
   for (int ix = 0; ix < nModelBinsX; ix++) {
      for (int iy = 0; iy < nModelBinsY; iy++) {
	 for (int iz = 0; iz < nModelBinsZ; iz++) {
	    TString canName = TString::Format("c_TimeBin%d__R%d_PHI%d_DT%d",TimeBinNum,ix+1,iy+1,iz+1);
	    c = (TCanvas*)f->Get(canName.Data());
	    c->Draw();
	    c->SetWindowSize(1200,600);
	    if (ix+iy+iz == (nModelBinsX-1)+(nModelBinsY-1)+(nModelBinsZ-1) ) {
	        c->SaveAs("myCoolGif.gif++50++");
	    } else {
	       c->SaveAs("myCoolGif.gif++50");
	    }
	    c->SaveAs(canName + ".gif");
	 }
      }
   }*/
   
}
