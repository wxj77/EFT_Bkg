//custom lib
#include "../../Parameters.h"

//c lib
#include <iostream>
#include <math.h>

//root lib
#include "TFile.h"
#include "TH2F.h"

// roo fit lib
#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPolynomial.h"
#include "RooHistPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"

////////////////////////////////////////////////////////////
using namespace RooFit ;

using namespace std;

int extendAccidentals(TString infile="../PDFs/Bkg_Accidentals_S1Log10S2_TH2F_origin.root", TString outfile="../PDFs/Bkg_Accidentals_S1Log10S2_TH2F.root", int TimeBinNum=1){

	TFile * in_root = TFile::Open(infile.Data(), "read");
	TFile * out_root = TFile::Open(outfile.Data(),"recreate");
	TH2F * ac = (TH2F*) (in_root->Get("Accidentals_S1vslog10S2"));
	//cout<<ac->GetEntries()<<endl;: 

	TH2F * acc = new TH2F("test","test", 300, 0, 300, 120., 0, 6.);
	acc->SetName("Accidentals_S1vslog10S2");
	for (int ii=0; ii< 300; ii++) {
		for (jj=0; jj< 120; jj++) {	
			acc->SetBinContent(ii+1, jj+1, 0.);
		}
	}
	float sum = 0;
	for (int ii=0; ii< ac->GetNbinsX(); ii++) {
                for (jj=0; jj< ac->GetNbinsY(); jj++) {          
                        acc->SetBinContent(ii+1+1, jj+40+1, ac->GetBinContent(ii+1, jj+1));
			sum+=ac->GetBinContent(ii+1, jj+1);
                }   
        } 
	cout << "total counts: "<< sum<<endl;
	RooRealVar x("x","x",S1MIN,S1MAX) ;
	RooRealVar y("y", "y", LOG10S2MIN, LOG10S2MAX);
	x.setBins(NS1BIN);
	y.setBins(NLOGS2BIN);
	//cout<<S1MIN<<S1MAX<<NS1BIN<<", "<<LOG10S2MIN<<LOG10S2MAX<<NLOGS2BIN<<endl;

	RooDataHist* r_ac= new RooDataHist("AccHist", "Accidentals Hist", RooArgSet(x,y), acc, 1.0);
	//cout<<r_ac->Print("v")<<endl;
	//cout<<"RooHist count:"<<r_ac->numEntries()<<endl;
	//cout<<"acc count: "<<int(acc->GetEntries())<<endl;

	RooHistPdf* histpdf = new RooHistPdf("PDF","Accidentals PDF",RooArgSet(x,y),*r_ac,2) ;
	//cout<<"expected events: "<<int(histpdf->expectedEvents(RooArgSet(x,y)))<<endl;
	RooDataHist& r_ac_new_o = histpdf->dataHist();
	//RooDataHist * r_ac_new= (RooDataHist*) (&r_ac_new_o);
	int NumGen = 10000000;
	RooDataHist * r_ac_new= (RooDataHist*) (histpdf->generateBinned(RooArgSet(x,y), NumGen ) );
	//RooDataHist * r_ac_new= (RooDataHist*) (histpdf->generateBinned(RooArgSet(x,y), int(ac->GetEntries()) ) );
	TH2F* ac_new = r_ac_new->createHistogram("Accidentals_S1vslog10S2",x,Binning(NS1BIN),YVar(y,Binning(NLOGS2BIN) ) );
	ac_new->Scale( float(sum)/float(NumGen));
	ac_new->SetName("Accidentals_S1vslog10S2");
	cout<<ac_new->GetEntries()<<",  "<<endl;//cout<<ac_new->GetBinWidth(1)<<endl;
	out_root->cd();
	ac_new->Write();
//	acc->Write();
	out_root->Close();
	return 1;
}
