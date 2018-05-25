# -*- coding: utf-8 -*-
"""
Created on Tue May 22 13:52:07 2018

@author: wxj
"""

# this creates the input files for JPM algorithm
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
import random

for luxstamp in [20141006, 20150226, 20150722, 20160222, ]: # times for 4 timebins
    line = workdir + "Uniform_%08d_JPM.txt"%(luxstamp)
    A=np.loadtxt(line, skiprows=0)
    xin= A[:,0]    #cm
    yin = A[:,1] #cm
    zin = A[:,2]    
    s2x = A[:,3]  
    s2y = A[:,4]  
    drift = A[:,5]  
    nInterp = A[:,6]  
    distInterp = A[:,7]  
    efield = A[:,8]  
    fig = plt.figure(luxstamp)
    ax = fig.add_subplot(1,1,1)
   # a = np.histogram2d()
    ax.hist2d (s2x**2+s2y**2, drift, bins=100, range=[[0,400], [0, 400]])
#    ax.hist2d (xin**2+yin**2, zin, bins=100)
 
for dt in range(1,5):
    dtsel = (40+(dt-1)*65 < drift) & (drift <=40+(dt)*65)
    a = efield[dtsel]
    print "drift bin: %d mean: %.1f std: %.1f [V/cm]"%(dt, np.median(a)/100, np.std(a)/100)
