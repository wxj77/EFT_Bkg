double counts=0;

for (int ii=0; ii<12; ii++){
  for (int jj=0; jj<4; jj++){
//    for (int kk=15; kk<20; kk++){
//      double val= DT4->GetBinContent(ii+1,jj+1,kk+1);
    for (int kk=0; kk<5; kk++){
      double val= DT1->GetBinContent(ii+1,jj+1,kk+1);
      if (val==0){cout<<"ii, jj, kk, value"<<"\t"<<ii+1<<"\t"<<jj+1<<"\t"<<kk+1<<"\t"<<val<<endl;counts+=1;}
    }
  }

} 
cout<<"fraction of zero: "<<counts/12./4./20.<<endl;

double width=4;
DT1->SetLineColor(kRed+2);
DT1->SetMarkerColor(kRed+2);
DT1->SetLineWidth(width);
DT2->SetLineColor(kGreen+2);
DT2->SetMarkerColor(kGreen+2);
DT2->SetLineWidth(width);
DT3->SetLineColor(kBlue+2);
DT3->SetMarkerColor(kBlue+2);
DT3->SetLineWidth(width);
DT4->SetLineColor(kMagenta+2);
DT4->SetMarkerColor(kMagenta+2);
DT4->SetLineWidth(width);

double hmin=0, hmax=0;
if (hmax< DT1->GetMaximum()) {hmax= DT1->Project3D("z")->GetMaximum();}
if (hmax< DT2->GetMaximum()) {hmax= DT2->Project3D("z")->GetMaximum();}
if (hmax< DT3->GetMaximum()) {hmax= DT3->Project3D("z")->GetMaximum();}
if (hmax< DT4->GetMaximum()) {hmax= DT4->Project3D("z")->GetMaximum();}

DT4->Project3D("z")->GetYaxis()->SetRangeUser(hmin, hmax);
TH1F* z1 = (TH1F*) DT4->Project3D("z")
z1->Draw();
DT1->Project3D("z")->Draw("same");
DT2->Project3D("z")->Draw("same");
DT3->Project3D("z")->Draw("same");
DT4->Project3D("z")->Draw("same");



//------------------------------------------------
//---------create the z projection of a histogram
//------------------------------------------------
/*
TH2F* DT1 = (TH2F*) _file0->Get("DT1");
TH2F* DT2 = (TH2F*) _file0->Get("DT2");
TH2F* DT3 = (TH2F*) _file0->Get("DT3");
TH2F* DT4 = (TH2F*) _file0->Get("DT4");
*/

int timeBin=1;
TString sName = "RnKr";
TString filename = TString::Format("h_TB%d_%s.root", timeBin, sName.Data());
TFile * newfile = new TFile(filename.Data(), "update");

TH3F* DT1 = (TH3F*) _file0->Get("DT1");
TH3F* DT2 = (TH3F*) _file0->Get("DT2");
TH3F* DT3 = (TH3F*) _file0->Get("DT3");
TH3F* DT4 = (TH3F*) _file0->Get("DT4");

DT1->SetLineColor(kRed+2);
DT1->SetMarkerColor(kRed+2);
DT1->SetLineWidth(width);
DT2->SetLineColor(kGreen+2);
DT2->SetMarkerColor(kGreen+2);
DT2->SetLineWidth(width);
DT3->SetLineColor(kBlue+2);
DT3->SetMarkerColor(kBlue+2);
DT3->SetLineWidth(width);
DT4->SetLineColor(kMagenta+2);
DT4->SetMarkerColor(kMagenta+2);
DT4->SetLineWidth(width);

TH1F* h_drift_dt1 = (TH1F*) DT1->Project3D("z");
TH1F* h_drift_dt2 = (TH1F*) DT2->Project3D("z");
TH1F* h_drift_dt3 = (TH1F*) DT3->Project3D("z");
TH1F* h_drift_dt4 = (TH1F*) DT4->Project3D("z");

h_drift_dt1->SetName("h_drift_dt1");
h_drift_dt2->SetName("h_drift_dt2");
h_drift_dt3->SetName("h_drift_dt3");
h_drift_dt4->SetName("h_drift_dt4");

TString histname = TString::Format("h_drift_TB%d_%s", timeBin, sName.Data());
//TH1F* h_drift_ComptonRest= new TH1F("h_drift_ComptonRest", "; drift time [us]; [cts kg^{-1} day^{-1}]");
//TH1F* h_drift_ComptonBottom= new TH1F("h_drift_ComptonBottom", "; drift time [us]; [cts kg^{-1} day^{-1}]");
TH1F* h_drift= new TH1F(histname.Data(), "; drift time [us]; event rate [cts kg^{-1} day^{-1}]", 20, 40, 300);
for (int ii=1; ii<6; ii++) {
    double value =h_drift_dt1->GetBinContent(ii); 
    h_drift->SetBinContent(ii, value);
}
for (int ii=1; ii<6; ii++) {
    double value =h_drift_dt2->GetBinContent(ii+5); 
    h_drift->SetBinContent(ii+5, value);
}
for (int ii=1; ii<6; ii++) {
    double value =h_drift_dt3->GetBinContent(ii+10); 
    h_drift->SetBinContent(ii+10, value);
}



for (int ii=1; ii<6; ii++) {
    double value =h_drift_dt4->GetBinContent(ii+15); 
    h_drift->SetBinContent(ii+15, value);
}

h_drift->Draw("same");

newfile->Write();
newfile->Close();


