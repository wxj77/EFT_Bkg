#!/bin/bash
option=99
if [ $# \> 0 ]; then
  option=$1
fi
echo "option: " ${option}
# option: 
#       0 : skip 1) libnest generation 2) read libnest result to root 
#       99 : full, do not skip anything 

cd UsefulBashScripts

bash GENERATE_Uniform.sh ${option}
bash GENERATE_8B.sh ${option}
bash GENERATE_Accidental.sh 
bash GENERATE_Ar37.sh ${option}
bash GENERATE_RnKr.sh ${option}
bash GENERATE_ComptonBottom.sh ${option}
bash GENERATE_ComptonRest.sh ${option}
bash GENERATE_Kr83.sh ${option}
#bash GENERATE_Xe127.sh ${option}
#bash GENERATE_neutron.sh

cd ..
