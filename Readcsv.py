#readcsv from dev

import csv
import numpy as np
import matplotlib.pyplot as plt

livetime = np.zeors(1); # in second?
deadtime = np.zeros(1)
S1 = np.zeros([1,1000])

livetimes= []
deadtimes=[]
S1s = np.zeros([0, 1000]);
S1srate = np.zeros([0, 1000]);

with open('/home/wei/Downloads/SlackDownloads/IsoS1S2/total_iso_s1_spect_real1.csv', 'rb') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        lt=float(row[2]);
        dt=float(row[3]);
        livetimes.append(lt);
        deadtimes.append(lt);
        for jj in range(1000):
            S1[0][jj] = float(row[jj+4])
        S1s = np.concatenate((S1s, S1), axis=0);
        S1srate = np.concatenate((S1srate, S1/lt), axis=0);

livetimes = np.array(livetimes)

deadtimes = np.array(deadtimes)

S1srate_mean = np.zeros(S1srate.shape[1])
S1srate_median = np.zeros(S1srate.shape[1])

for jj in range(S1srate_mean.shape[0]):
    S1srate_mean[jj] = np.mean(S1srate[:,jj])
    S1srate_median[jj] = np.median(S1srate[:,jj])

fig = plt.figure(1)
ax = fig.add_subplot(1,1,1)
ax.plot(S1srate_mean)
ax.yscale('log')
ax.xscale('log')

