# EFT_Bkg

This repositary is to record how the background for EFT.

**For running EFT in LUXLimit Code.** 

1. copy folder PDFs /LUXLimits/EFTRun4/BackgroundModel/PDFs
  
  cp -r PDFs my_path_to/LUXLimits/EFTRun4/BackgroundModel/PDFs

2. follow the instructions in  /LUXLimits/EFTRun4/README.md
 
**Description of the content of this folder.**

1. PDFs

  paramBkg.C : parameter that records the expected value of the activities in unit of rate per kg per day, and energy spectrum of the background components.
  Bkg_${histogramType}_${sourceName}_TB${timeBin}.root
  histogramType:
    value: S1Log10S2_TH2F, S1S2_TH2F, rphidrift_TH3F
  sourceName: name of the source.
    value: 8B, Accidentals, Ar37, RnKr, ComptonBottom, ComptonRest, ?"Kr83", ?"Xe127", ?"neutron"(Shaun)
  timeBin: number of time bins.
    value: 1, 2, 3, 4

  in each Bkg_${histogramType}_${sourceName}_TB${timeBin}.root file, there is a list of histograms for drift bins. The type of histogram is depending on the file. 
      TH2F DT1, DT2, DT3, DT4
  or  TH3F DT1, DT2, DT3, DT4

2. notes.txt

  describe the details of the location of the number of expected values is from.  
   
