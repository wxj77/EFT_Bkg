#!/bin/bash

#script that runs over the .dat files to produce a libNEST generated bkg model
#input: directory to a component isotope e.g. /cms26r0/sjh/lux_bkg_model/LUXR4BackgroundFull/PMTMOUNT/Co60/
#output: a .root file that has all the events from the input directory in it

#basedir=/net/cms26/cms26r0/sjh/bkg_model_sims/Run4BKG_Summary/
#basedir=/net/cms26/cms26r0/sjh/Run4BKG_Summary/
#basedir=/net/cms26/cms26r0/lmc/lux_bkg_model_sims/
#basedir=/net/cms26/cms26r0/sjh/Run4BKG_Summary/

basedir_array=( /net/cms26/cms26r0/lux/Run4BKG_Summary_C3PO_processed_timeBin1/ /net/cms26/cms26r0/lux/Run4BKG_Summary_C3PO_processed_timeBin2/ /net/cms26/cms26r0/lux/Run4BKG_Summary_C3PO_processed_timeBin3/ /net/cms26/cms26r0/lux/Run4BKG_Summary_C3PO_processed_timeBin4/ )

#basedir_array=( /net/cms26/cms26r0/sjh/Run4BKG_Summary_JPM_processed_timeBin1/ )
timebin=0
for basedir in ${basedir_array[@]}
do
   timebin=$((timebin + 1))
   echo " "
   echo "================================"
   echo "Running for time bin $timebin..."
   for compo in `ls -d ${basedir}*/`
   do
      #echo "$compo"
      for dir in `ls -d ${compo}*/` #all directories
      do
      
         D2=$(dirname $dir)
         ISOTOPE=$(basename $dir)
         COMPO=$(basename $D2)
         
         #echo $dir
         echo "Running on..."
         echo "component: $COMPO"
         echo "isotope: $ISOTOPE"
         
         #./generateFastBkg $timebin ./root_output/tb${timebin}/${COMPO}_${ISOTOPE}_summary.root ${dir}
         #/net/cms2/cms2r0/Job/JobSubmit.csh ./generateFastBkg $timebin ./root_output/tb${timebin}/${COMPO}_${ISOTOPE}_summary.root ${dir}
         
         /net/cms2/cms2r0/Job/JobSubmit.csh ./generateFastBkg $timebin ./root_output/tb${timebin}/${COMPO}_${ISOTOPE}_summary.root ${dir}
         
         echo "Saving to ./root_output/tb${timebin}/${COMPO}_${ISOTOPE}_summary.root"
         echo " "
         
      done
   done
done
