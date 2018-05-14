void bkgWrapper () {
   TStopwatch timer;
   timer.Start();
   gROOT->ProcessLine(".include $NESTPATH");
   gROOT->ProcessLine(".L $NESTPATH/NEST.cxx+");
   
   gROOT->ProcessLine(".L generate_Bkg_S1Log10S2_TH2F.C+");
   
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F(1000000,1)");
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F_Ar37(500000,1)");
   
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F(1000000,2)");
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F_Ar37(500000,2)");
   
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F(1000000,3)");
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F_Ar37(500000,3)");
   
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F(1000000,4)");
   gROOT->ProcessLine("generate_Bkg_S1Log10S2_TH2F_Ar37(500000,4)");
   
   //gROOT->ProcessLine(".x generate_Bkg_S1Log10S2_TH2F.C+(10,4)");
   cout << "---------------------------------------------------------------" << endl;
   cout << "Finished running in " << timer.RealTime() << " seconds." << endl;
   cout << "---------------------------------------------------------------" << endl;
}
