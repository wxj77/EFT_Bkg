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
sName="ER"

:'
cd ./ReadtestNESTToROOT
make clean
make ReadtestNESTToROOT
cd ..
'
if [ ${option} \> 90 ]; then
echo "long run: generate libnest result and read it to root"
for TB in 1 2 3 4; do
#  ../LibNEST_TB${TB}/testNEST 1000000 beta 0 130 -1 -1,-1,-1 >../Data/${sName}_TB${TB}_newlibnest.dat
#  ../LibNEST_TB${TB}/testNEST 1000000 gamma 0 130 -1 -1,-1,-1 >../Data/${sName}_TB${TB}_newlibnest.dat
  ../LibNEST_TB${TB}/testNEST 1000000 weighted 0 130 -1 -1,-1,-1 >../Data/${sName}_TB${TB}_newlibnest.dat
                                  
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



