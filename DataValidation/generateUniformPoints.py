# -*- coding: utf-8 -*-
"""
Created on Tue May 22 13:26:55 2018

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

nSims=10000; # number of sims
rmin, rmax=0,18; #cm
zmin, zmax= 8, 52 # cm
phimin, phimax = -np.pi, np.pi #
for luxstamp in [20141006, 20150226, 20150722, 20160222, ]: # times for 4 timebins
    result=np.empty((nSims,4), 'f4')
    for ii in range(nSims): 
        r2 = random.random()
        r = np.sqrt(r2)
        r = rmin+(rmax-rmin)*r
        phi = random.random()
        phi=phimin+(phimax-phimin)*phi
        z = random.random()
        z = zmin+(zmax-zmin)*z
        temp =np.array([r*np.cos(phi), r*np.sin(phi), z, luxstamp])
        #result=np.concatenate((result, np.array([temp])), axis=0)
        result[ii,:]=temp
    newfilename = workdir+"Uniform_%08d.txt"%(luxstamp)
    np.savetxt(newfilename, result, fmt="%.3f\t%.3f\t%.3f\t%08d\t")
            