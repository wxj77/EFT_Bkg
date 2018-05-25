#!/bin/bash

../LibNEST_TB1/testNEST 1000000 ER 0 130 -2 -2 1 1>test.data
option=99
if [ $# \> 0 ]; then
  option=$1
fi

#source name
sName="ER"
function GetLuxStamp(){
  if [ ${TB} \= 1 ]; then luxstamp=20141006; fi   
  if [ ${TB} \= 2 ]; then luxstamp=20150226; fi
  if [ ${TB} \= 3 ]; then luxstamp=20150722; fi
  if [ ${TB} \= 4 ]; then luxstamp=20160222; fi
}

echo "long run: generate jpm based libnest result. "
for TB in 1 2 3 4; do
  GetLuxStamp
  ../LibNEST_TB1/testNEST 1000000 ${sName} 0 130 -2 -2 1 ../DataValidation/Uniform_${luxstamp}_JPM.txt >../Data/${sName}_TB${TB}_newlibnest_jpm.dat                              
done

sName="NR"
for TB in 1 2 3 4; do
  GetLuxStamp
  ../LibNEST_TB1/testNEST 1000000 ${sName} 0 300 -2 -2 1 ../DataValidation/Uniform_${luxstamp}_JPM.txt >../Data/${sName}_TB${TB}_newlibnest_jpm.dat                              
done



