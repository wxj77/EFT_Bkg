# EFT_Bkg

This repositary is to record how the background for EFT.

**For running EFT in LUXLimit Code.** 

1. copy folder PDFs /LUXLimits/EFTRun4/BackgroundModel/PDFs

  # if you want to change parameter setting in Parameters.h and ~~paramBkgGen.C~~
  cp yout_path_to/Parameters.h .
  
  # make sure you have compiled the newest nest model in LibNEST_TB1, LibNEST_TB2, LibNEST_TB3, LibNEST_TB4 folders, to represent the 4 time bins that is used for generate background model. Now they are all submodules link to LibNEST repo at git@github.com:luxdarkmatter/LibNEST.git.
  git submodule init
  git submodule update
  switch to the correct detector setting. testNEST.cpp: line 46-49
  comment unnessary outputs: testNEST.cpp: line 140-148, 354-395, NEST.cpp 469-474
  # to compile LibNEST, follow instructions in LibNEST
  
  ~~you need to manually change paramBkgGen.C to give you the S1 S2 LogS2 bins that you want.~~ 
  bash GENERATE_ALL.sh  
  # this will regenerate all backgrounds if it is possible.

  cp -r PDFs my_path_to/LUXLimits/EFTRun4/BackgroundModel/PDFs

2. follow the instructions in  /LUXLimits/EFTRun4/README.md
 
**Description of the content of this folder.**

1. PDFs

  paramBkg.C : parameter that records the expected value of the activities in unit of rate per kg per day, and energy spectrum of the background components.
  Bkg_${histogramType}_${sourceName}_TB${timeBin}.root
  histogramType:
    value: S1Log10S2_TH2F, S1S2_TH2F, rphidrift_TH3F
  sourceName: name of the source.
    value: Uniform, 8B, Accidental, Ar37, RnKr, ComptonBottom, ComptonRest, ?"Kr83", ?"Xe127", ?"neutron"(Shaun)
  timeBin: number of time bins.
    value: 1, 2, 3, 4

  in each Bkg_${histogramType}_${sourceName}_TB${timeBin}.root file, there is a list of histograms for drift bins. The type of histogram is depending on the file. 
      TH2F DT1, DT2, DT3, DT4
  or  TH3F DT1, DT2, DT3, DT4

2. UsefulBashScripts

  // not exist yet
  GENERATE_Uniform.sh
  GENERATE_8B.sh
  GENERATE_Accidental.sh
  GENERATE_Ar37.sh
  GENERATE_RnKr.sh
  GENERATE_ComptonBottom.sh
  GENERATE_ComptonRest.sh
  GENERATE_Kr83.sh
//  GENERATE_Xe127.sh
//  GENERATE_neutron.sh

3. Data
  
  Data that I used to feed the histogram distributions.
  // not perfect yet, probably will never be perfect.
  
  
99. notes.txt

  describe the details of the location of the number of expected values is from.  
   
