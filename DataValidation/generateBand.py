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

offset=0
bandtype="ER"
for bandtype in ["ER", "NR"]:
    for tb in range(1,5):
        filename = workdir+ "../Data/%s_TB%d_newlibnest.dat"%(bandtype,tb)
        filename = workdir+ "../Data/%s_TB%d_newlibnest_jpm.dat"%(bandtype,tb)
        offset=14
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
        
        xin= A[:,offset+0]    #cm
        yin = A[:,offset+1] #cm
        zin = A[:,offset+2]    
        s2x = A[:,offset+3]  
        s2y = A[:,offset+4]  
        drift = A[:,offset+5]  
        nInterp = A[:,offset+6]  
        distInterp = A[:,offset+7]  
        efield = A[:,offset+8]  
        
        temp =np.array([1.5, 0,  0,  0])
        temp2 =np.array([1.5, 0,  0,  0])
        for dt in range(1,5):
            scalefactor=1.
            #if ((bandtype=="NR") | (bandtype=="ER") ):
            if(0):
                if (tb==1): scalefactor=33.002645387/1.173/26.4
                if (tb==2): scalefactor=34.0981667342/1.173/26.
                if (tb==3): scalefactor=34.1595575843/1.173/25.8
                if (tb==4): scalefactor=33.4370744606/1.173/25.3
            result=np.empty((0,4), 'f4')
            result2=np.empty((0,4), 'f4')
            dtsel = (40+(dt-1)*65 < tDrift) & (tDrift <=40+(dt)*65)
            if (offset==14):
                dtsel = (40+(dt-1)*65 < drift) & (drift <=40+(dt)*65)
            for ii in range(1,300):
                s1sel = (ii<S1_Zcorr) & (S1_Zcorr<=ii+1)
                sel=dtsel & s1sel
                s2 = S2_Zcorr[sel]/scalefactor#*np.exp(-drift[sel]/800)/np.exp(-tDrift[sel]/800)
                s2s1 = S2_Zcorr[sel]/S1_Zcorr[sel]/scalefactor
        #        print ii+.5, np.percentile(s2, 10),  np.percentile(s2, 50),  np.percentile(s2, 90)
                if len(s2)!=0:
                    temp =np.array([ii+.5, np.percentile(s2, 10),  np.percentile(s2, 50),  np.percentile(s2, 90)])
                    temp2 =np.array([ii+.5, np.percentile(s2s1, 10),  np.percentile(s2s1, 50),  np.percentile(s2s1, 90)])
                result=np.concatenate((result, np.array([temp])), axis=0)
                result2=np.concatenate((result2, np.array([temp2])), axis=0)
    #        plt.plot(result[:,0], np.log10(result[:,2]), label="ER mean")
    #        plt.plot(result[:,0], np.log10(result[:,1]), label="ER 10%")
    #        plt.plot(result[:,0], np.log10(result[:,3]), label="ER 90%")
            newfilename = workdir+ bandtype+"_TB%d_DT%d.txt"%(tb, dt)
            np.savetxt(newfilename, result)
            newfilename2 = workdir+ bandtype+"P_TB%d_DT%d.txt"%(tb, dt)
            np.savetxt(newfilename2, result2)
            a = S2_rawArea/NeExtr
            a = a[(a>10)&(a<50)]
            print "time bin: ",tb,"drift bin: ", dt, "average of se:",  np.median(a) 
            #http://teacher.pas.rochester.edu:8080/wiki/pub/Lux/LuxDB00000489/libNEST_ER_Model_Parameters.pdf
            # from jeremy the se size should be [phd]: timebin 1: 26.4, timebin2: 26 time bin 3: 25.8 time bin 4: 25.3
            
            
            
    
