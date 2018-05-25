//
// LUX_Run4.hh
//
// Adapted from Quentin Riffard by Jacob Cutter, May 8, 2018
//
// This header file serves as a template for creating one's own VDetector class.
// This will be ultimately used to customize the NEST detector parameters to meet
// individual/collaboration needs.
//
// Note that the detector parameters can also be varied throughout a run, etc.

#ifndef LUX_Run4_hh
#define LUX_Run4_hh 1

#include "VDetector.hh"

using namespace std;


class LUX_Run4: public VDetector {

public:

	LUX_Run4() {
		cerr << "*** You are currently using the default LUX Run 4 (time bin 1) detector settings. ***" << endl << endl;

		// Call the initialisation of all the parameters
		Initialization();
	};
	virtual ~LUX_Run4() {};

	// Do here the initialization of all the parameters that are not varying as a function of time
	virtual void Initialization() {
		// Primary Scintillation (S1) parameters
		g1 = 0.1043; //phd per S1 phot at dtCntr (not phe). Divide out 2-PE effect
		sPEres = 0.37; //single phe resolution (Gaussian assumed)
		sPEthr = (0.3*1.173)/0.915; //POD threshold in phe, usually used IN PLACE of sPEeff
		sPEeff = 1.00; //actual efficiency, can be used in lieu of POD threshold
		noise[0] = -0.01; //baseline noise mean and width in PE (Gaussian)
		noise[1] = 0.08; //baseline noise mean and width in PE (Gaussian)
		P_dphe = 0.173; //chance 1 photon makes 2 phe instead of 1 in Hamamatsu PMT

		coinLevel= 2; //how many PMTs have to fire for an S1 to count
		numPMTs = 118; //For coincidence calculation

		// Ionization and Secondary Scintillation (S2) parameters
		g1_gas = 0.08845; //phd per S2 photon in gas, used to get SE size
		s2Fano = 0.; //Fano-like fudge factor for SE width
		s2_thr = 0.; //the S2 threshold in phe or PE, *not* phd. Affects NR most
		S2botTotRatio = 0.449; //bottom-to-total, typically only used for position recon (1-this)
		E_gas = 7.5; //field in kV/cm between liquid/gas border and anode
		eLife_us = 735.; //the drift electron mean lifetime in micro-seconds

		// Thermodynamic Properties
		inGas = false;
		T_Kelvin = 177.; //for liquid drift speed calculation
		p_bar = 1.95; //gas pressure in units of bars, it controls S2 size
		//if you are getting warnings about being in gas, lower T and/or raise p

		// Data Analysis Parameters and Geometry
		dtCntr = 162.535; //center of detector for S1 corrections, in usec.
		dt_min = 40.; //minimum. Top of detector fiducial volume
		dt_max = 300.; //maximum. Bottom of detector fiducial volume

		radius = 180.; //millimeters

		TopDrift = 544.2198; //mm not cm or us (but, this *is* where dt=0)
		//a z-axis value of 0 means the bottom of the detector (cathode OR bottom PMTs)
		//In 2-phase, TopDrift=liquid/gas border. In gas detector it's GATE, not anode!
		anode = 549.2468; //the level of the anode grid-wire plane in mm
		//In a gas TPC, this is not TopDrift (top of drift region), but a few mm above it
		gate = 539.1928; //mm. This is where the E-field changes (higher)
		// in gas detectors, the gate is still the gate, but it's where S2 starts
		cathode = 56.; //mm

		// 2-D (X & Y) Position Reconstruction
		PosResExp = 0.015; // exp increase in pos recon res at hi r, 1/mm
		PosResBase = 70.8364; // baseline unc in mm, see NEST.cpp for usage
	}

	//S1 PDE custom fit for function of z
	//s1polA + s1polB*z[mm] + s1polC*z^2+... (QE included, for binom dist) e.g.
	virtual double FitS1 ( double xPos_mm, double yPos_mm, double zPos_mm ) {
		double radius = sqrt(pow(xPos_mm,2.)+pow(yPos_mm,2.));
		double amplitude = 307.9-0.3071*zPos_mm+0.0002257*pow(zPos_mm,2.);
		double shape = 1.1525e-7*sqrt(fabs(zPos_mm-318.84));
		//3.7393e-11*pow(zPos_mm-316.42,2.);
		return -shape * pow ( radius, 3. ) + amplitude;
	}
		
	//Drift electric field as function of Z in mm
	//For example, use a high-order poly spline
	virtual double FitEF ( double xPos_mm, double yPos_mm, double zPos_mm ) { // in V/cm
  	// Drift field polynomial, fit to time bin 1
		return 126.120812197 
			-1.90341778291*zPos_mm 
			+0.0221484569642*pow(zPos_mm, 2.0) 
			-9.87866796872e-5*pow(zPos_mm, 3.0) 
			+2.0828842974e-7*pow(zPos_mm, 4.0) 
			-1.57249357722e-10*pow(zPos_mm, 5.0);
	}

	//S2 PDE custom fit for function of r
	//s2polA + s2polB*r[mm] + s2polC*r^2+... (QE included, for binom dist) e.g.
	virtual double FitS2 ( double xPos_mm, double yPos_mm ) {
		double radius = sqrt(pow(xPos_mm,2.)+pow(yPos_mm,2.));

		return 9156.3
			+6.22750*pow(radius,1.)
			+0.38126*pow(radius,2.)
			-0.017144*pow(radius,3.)
			+0.0002474*pow(radius,4.)
			-1.6953e-6*pow(radius,5.)
			+5.6513e-9*pow(radius,6.)
 			-7.3989e-12*pow(radius,7.);
	}
	
	// Vary parameters as necessary based on the timestamp of the event, or any other
	// custom dependencies. Any protected parameters from VDetector can be modified here.
	
	virtual void SetParameters_TimeBin1() {
		// Primary Scintillation (S1) parameters
		set_g1(0.1043);
		set_sPEres(0.37);
		set_sPEthr((0.3*1.173)/0.915);
		set_sPEeff(1.0);
		set_noise(-0.01, 0.08);
		set_P_dphe(0.173);

		set_coinLevel(2);
		set_numPMTs(118);

		// Ionization and Secondary Scintillation (S2) parameters
		set_g1_gas(0.08845/(33.002645387/1.173/26.4));
		set_s2Fano(0.);
		set_s2_thr(0.);
		set_S2botTotRatio(0.449);
		set_E_gas(7.5);
		set_eLife_us(735.);

		// Thermodynamic Properties
		set_inGas(false);
		set_T_Kelvin(177.);
		set_p_bar(1.95);

		// Data Analysis Parameters and Geometry
		set_dtCntr(162.535);
		set_dt_min(40.);
		set_dt_max(300.);
		set_radius(180.);
		set_TopDrift(544.2198);
		set_anode(549.2468);
		set_gate(539.1928);
		set_cathode(56.);

		// 2-D (X & Y) Position Reconstruction
		set_PosResExp(0.015);
		set_PosResBase(70.8364);
	}

	virtual void SetParameters_TimeBin2() {
		// Primary Scintillation (S1) parameters
		set_g1(0.1013);
		set_sPEres(0.37);
		set_sPEthr((0.3*1.173)/0.915);
		set_sPEeff(1.0);
		set_noise(-0.01, 0.08);
		set_P_dphe(0.173);

		set_coinLevel(2);
		set_numPMTs(118);

		// Ionization and Secondary Scintillation (S2) parameters
		set_g1_gas(0.08708/(34.0981667342/1.173/26.));
		set_s2Fano(0.2);
		set_s2_thr(0.);
		set_S2botTotRatio(0.449);
		set_E_gas(7.73);
		set_eLife_us(947.);

		// Thermodynamic Properties
		set_inGas(false);
		set_T_Kelvin(177.);
		set_p_bar(1.95);

		// Data Analysis Parameters and Geometry
		set_dtCntr(162.535);
		set_dt_min(40.);
		set_dt_max(300.);
		set_radius(180.);
		set_TopDrift(544.2198);
		set_anode(549.2468);
		set_gate(539.1928);
		set_cathode(56.);

		// 2-D (X & Y) Position Reconstruction
		set_PosResExp(0.015);
		set_PosResBase(70.8364);
	}

	virtual void SetParameters_TimeBin3() {
		// Primary Scintillation (S1) parameters
		set_g1(0.1013);
		set_sPEres(0.37);
		set_sPEthr((0.3*1.173)/0.915);
		set_sPEeff(1.0);
		set_noise(-0.01, 0.08);
		set_P_dphe(0.173);

		set_coinLevel(2);
		set_numPMTs(118);

		// Ionization and Secondary Scintillation (S2) parameters
		set_g1_gas(0.086/(34.1595575843/1.173/25.8));
		set_s2Fano(1.14);
		set_s2_thr(0.);
		set_S2botTotRatio(0.449);
		set_E_gas(7.8);
		set_eLife_us(871.);

		// Thermodynamic Properties
		set_inGas(false);
		set_T_Kelvin(177.);
		set_p_bar(1.95);

		// Data Analysis Parameters and Geometry
		set_dtCntr(162.535);
		set_dt_min(40.);
		set_dt_max(300.);
		set_radius(180.);
		set_TopDrift(544.2198);
		set_anode(549.2468);
		set_gate(539.1928);
		set_cathode(56.);

		// 2-D (X & Y) Position Reconstruction
		set_PosResExp(0.015);
		set_PosResBase(70.8364);
	}

	virtual void SetParameters_TimeBin4() {
		// Primary Scintillation (S1) parameters
		set_g1(0.099274);
		set_sPEres(0.37);
		set_sPEthr((0.3*1.173)/0.915);
		set_sPEeff(1.0);
		set_noise(-0.01, 0.08);
		set_P_dphe(0.173);

		set_coinLevel(2);
		set_numPMTs(118);

		// Ionization and Secondary Scintillation (S2) parameters
		set_g1_gas(0.08473/(33.4370744606/1.173/25.3));
		set_s2Fano(1.15);
		set_s2_thr(0.);
		set_S2botTotRatio(0.449);
		set_E_gas(7.768);
		set_eLife_us(871.);

		// Thermodynamic Properties
		set_inGas(false);
		set_T_Kelvin(177.);
		set_p_bar(1.95);

		// Data Analysis Parameters and Geometry
		set_dtCntr(162.535);
		set_dt_min(40.);
		set_dt_max(300.);
		set_radius(180.);
		set_TopDrift(544.2198);
		set_anode(549.2468);
		set_gate(539.1928);
		set_cathode(56.);

		// 2-D (X & Y) Position Reconstruction
		set_PosResExp(0.015);
		set_PosResBase(70.8364);
	}

	// Vary parameters as necessary based on the timestamp of the event, or any other
	// custom dependencies. Any protected parameters from VDetector can be modified here.
	virtual void SetTime(unsigned int timestamp) {
		// if timestamp falls in certain range...
		set_g1(0.0760);
	}

};


#endif
