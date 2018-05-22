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
sName="ComptonBottom"

#use spacial distribution from R4
for f in ../PDFs/default_frombackup/*TH3F_ComptonBottom*.root; do
  fnew=${f/default_frombackup/}
  echo ${fnew}
  cp ${f} ${fnew}
done

#use energy uniform distribution
for f in ../PDFs/*TH2F_Uniform*.root; do
  fnew=${f/Uniform/${sName}}
  echo ${fnew}
  cp ${f} ${fnew}
done


