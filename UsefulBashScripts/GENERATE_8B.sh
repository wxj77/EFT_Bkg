#!/bin/bash

#source name
sName="8B"

#use spacial uniform distribution
for f in ../PDFs/*TH3F_Uniform*.root; do
  fnew=${f/Uniform/${sName}}
  echo ${fnew}
  cp ${f} ${fnew}
done

#use energy uniform distribution
#for f in ../PDFs/*TH2F_Uniform*.root; do
#  fnew=${f/Uniform/${sName}}
#  echo ${fnew}
#  cp ${f} ${fnew}
#done

#use libnest dat to feed histogram for energy spectrum
# make sure you compile NEST first and put it the same layer with EFT_Bkg folder.

cd ./ReadtestNESTToROOT
make clean
make ReadtestNESTToROOT
for TB in 1 2 3 4; do
  # generate results from NEST software and save to dat files
  # seed for NEST model to use
  # 8B: B8 , Ar37: 2.8keV, Kr83: 42keV
  # Xe127: 0.096 4keV(l) + 0.715 28.6 keV(kalpha) + 0.1339 32.5keV(kbeta1) + 0.0303(rest) 33 keV 
  # http://www.nucleide.org/DDEP_WG/Nuclides/Xe-127_tables.pdf
  # RnKr, ComptonBottom, ComptonRest: uniform energy between 0 to 100 keV.
  # read dat files to root
  ./ReadtestNESTToROOT ../../Data/8Bout_TB${TB}.dat ../../Data/8B_TB${TB}.root
  # fill root results to s1s2, s1log10s2 histoogram
  root -q -b -l "./ReadROOTToTH2F.cpp(\"../../Data/8B_TB${TB}.root\", \"8B\", ${TB})"
done 

cd ..

