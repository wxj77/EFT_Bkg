# -*- coding: utf-8 -*-
"""
Created on Mon May 21 18:13:16 2018

@author: wxj
"""
import os, sys

sys.path.insert(0, "/home/wxj/.2go")
sys.path.insert(0, "/home/wxj/.x2go")
sys.path.insert(0, "/home/wxj/.2go/PythonScripts/RunSetups")
sys.path.insert(0, "/home/wxj/.2go/PythonScripts")
sys.path.insert(0, "/home/wxj/gtest_result/")

workdir="/home/wxj/EFT_Bkg/DataValidation/"
import numpy as np
import StringIO
import matplotlib.pyplot as plt

bandtype="ER"
for bandtype in ["ER", "NR"]:
    for tb in range(1,5):
        filename = workdir+ "../Data/%s_TB%d_newlibnest.dat"%(bandtype,tb)
        with open(filename) as f:
            A=np.loadtxt((x.replace(b',',b' ') for x in f), skiprows=4)
        
        E = A[:,0]    #keV
        field = A[:,1] #V/cm
        tDrift = A[:,2]    
        X = A[:,3]  
        Y = A[:,4]  
        Z = A[:,5]  
        Nph = A[:,6]  
        Ne = A[:,7]  
        S1_raw = A[:,8]  
        S1_Zcorr = A[:,9]  
        S1c_spike= A[:,10]  
        NeExtr = A[:,11]  
        S2_rawArea = A[:,12]  
        S2_Zcorr = A[:,13]  
        
        
        for dt in range(1,5):
            result=np.empty((0,4), 'f4')
            dtsel = (40+(dt-1)*65 < tDrift) & (tDrift <=40+(dt)*65)
            for ii in range(1,300):
                s1sel = (ii<S1_Zcorr) & (S1_Zcorr<=ii+1)
                sel=dtsel & s1sel
                s2 = S2_Zcorr[sel]
        #        print ii+.5, np.percentile(s2, 10),  np.percentile(s2, 50),  np.percentile(s2, 90)
                temp =np.array([ii+.5, np.percentile(s2, 10),  np.percentile(s2, 50),  np.percentile(s2, 90)])
                result=np.concatenate((result, np.array([temp])), axis=0)
    #        plt.plot(result[:,0], np.log10(result[:,2]), label="ER mean")
    #        plt.plot(result[:,0], np.log10(result[:,1]), label="ER 10%")
    #        plt.plot(result[:,0], np.log10(result[:,3]), label="ER 90%")
            newfilename = bandtype+"_TB%d_DT%d.txt"%(tb, dt)
            np.savetxt(newfilename, result)
            
            
            
    
