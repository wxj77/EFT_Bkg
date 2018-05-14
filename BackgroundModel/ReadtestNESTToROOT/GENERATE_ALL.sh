# if not load ROOT gcc #
module unload ROOT
module load ROOT/5.34.20
module unload gcc
module load gcc/4.8.2

# B8 background is generated from new libnest, THE COUNT IS NOT CORRECT NOW. Before this step, you need to have a generated 8B for TB1,2,3,4 with name 8Bout_TB1,2,3,4. 
# S1 log10S2 spectrum
cd ./ReadtestNESTToROOT
make
for TB in 1 2 3 4; do
	./ReadtestNESTToROOT ../8Bout_TB${TB}.dat ../8B_TB${TB}.root
	./ReadROOTToTH2F ../8B_TB${TB}.root 8B_TB${TB} ${TB}
done 
cd ..

#KrRn, ComptonBottom, ComptonRest THE COUNT IS NOT CORRECT NOW.
# S1 log10S2 spectrum
root -b -l -q "responseTH2FromNtuples_new.C+()"
# r phi dt spectrum
root -b -l -q "spatialTH3FromNtuples_new.C+()"


#Ar37  THE COUNT IS NOT CORRECT NOW.
for TB in 1 2 3 4; do
	root -b -l -q "generate_Bkg_S1Log10S2_TH2F_Ar37.C+(10000, ${TB})"
done;

#Accidental, I don't know how to interpolate this.
