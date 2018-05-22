#!/bin/bash
option=99
if [ $# \> 0 ]; then
  option=$1
fi
echo "option: " ${option}
# option: 
#       0 : skip 1) libnest generation 2) read libnest result to root 
#       99 : full, do not skip anything 
#source name
sName="Uniform"

cd ./ReadtestNESTToROOT
make clean
make ReadtestNESTToROOT
cd ..


#use spacial uniform distribution
for f in ../PDFs/default_frombackup/*TH3F_Uniform*.root; do
  fnew=${f/default_frombackup/}
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

if [ ${option} \> 90 ]; then
echo "long run: generate libnest result and read it to root"
for TB in 1 2 3 4; do
  # generate results from NEST software and save to dat files
  # seed for NEST model to use
  # 8B: B8 , Ar37: 2.8keV, Kr83: 42keV
  # Xe127: 0.096 4keV(l) + 0.715 28.6 keV(kalpha) + 0.1339 32.5keV(kbeta1) + 0.0303(rest) 33 keV 
  # http://www.nucleide.org/DDEP_WG/Nuclides/Xe-127_tables.pdf
  # RnKr, ComptonBottom, ComptonRest: uniform energy between 0 to 100 keV.
#  ../LibNEST_TB${TB}/testNEST 10000000 8B 0 10 -1 -1,-1,-1 >../Data/${sName}_TB${TB}_newlibnest.dat
  ../LibNEST_TB${TB}/testNEST 100000 beta 0 70 -1 -1,-1,-1 >../Data/${sName}_TB${TB}_newlibnest.dat
                              
done

for TB in 1 2 3 4; do
  # read dat files to root
 # ./ReadtestNESTToROOT ../../Data/8Bout_TB${TB}.dat ../../Data/8B_TB${TB}.root
  # fill root results to s1s2, s1log10s2 histoogram
 # root -q -b -l "./ReadROOTToTH2F.cpp(\"../../Data/8B_TB${TB}.root\", \"8B\", ${TB})"
  ./ReadtestNESTToROOT/ReadtestNESTToROOT ../Data/${sName}_TB${TB}_newlibnest.dat ../Data/${sName}_TB${TB}_newlibnest.root
done


fi
 
cd ./ReadtestNESTToROOT
for TB in 1 2 3 4; do
  # fill root results to s1s2, s1log10s2 histoogram
  root -q -b -l "./ReadROOTToTH2F.cpp(\"../../Data/${sName}_TB${TB}_newlibnest.root\", \"${sName}\", ${TB})"
done 
cd ..



