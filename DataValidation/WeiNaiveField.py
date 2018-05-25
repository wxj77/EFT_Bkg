# -*- coding: utf-8 -*-
"""
Created on Thu May 24 16:46:57 2018

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

zPos_mm=np.linspace(40,600)
y1 =126.120812197 -1.90341778291*zPos_mm +0.0221484569642*(zPos_mm** 2.0) -9.87866796872e-5*(zPos_mm** 3.0) +2.0828842974e-7*(zPos_mm** 4.0) -1.57249357722e-10*(zPos_mm** 5.0);
plt.plot(zPos_mm,y1, label="electric field NEST")
#y2 = 126.120812197 -1.90341778291*zPos_mm +0.0221484569642*(zPos_mm** 2.0)
#plt.plot(zPos_mm,y2)
plt.axvline(56, label='cathode', c='k')
plt.axvline(544.2, label='liquid surface', c='r')
plt.legend()
