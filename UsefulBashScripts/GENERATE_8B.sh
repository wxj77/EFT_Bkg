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

cd ./ReadtestNESTToROOT
make clean
make ReadtestNESTToROOT
for TB in 1 2 3 4; do
  ./ReadtestNESTToROOT ../../Data/8Bout_TB${TB}.dat ../../Data/8B_TB${TB}.root
  root -q -b -l "./ReadROOTToTH2F.cpp(\"../../Data/8B_TB${TB}.root\", \"8B\", ${TB})"

done 

cd ..

