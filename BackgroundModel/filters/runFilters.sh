#!/bin/bash

array=( Ex_All Ex_Th232 FCS_Co60 FCS_Pb212 FCS_Ra226 FCS_Tl208 PMTB_All PMTB_Th232 PMTT_All PMTT_Th232 Xe_Kr85 Xe_Rn220 Xe_Rn222 Xe_Xe127 )
#array=( Xe_Kr85 )
#timebin=1

for timebin in `seq 1 4`
do
   for type in ${array[@]}
   do
      
      command="filter.C(\"$type\",$timebin)"
      /net/cms2/cms2r0/Job/JobSubmit.csh root -b -q $command
       
   done
done
