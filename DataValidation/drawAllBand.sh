#!/bin/bash

filename=/home/wxj/EFT_Bkg/DataValidation/eft_bg_data_trial_05182018.root
for tb in 1 2 3 4; do 
    for dt in 1 2 3 4; do 
        root -q -l -b "drawValidation.C(\"${filename}\",${tb}, ${dt})"
    done
done
