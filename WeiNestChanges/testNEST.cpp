/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testNEST.cpp
 * Author: brodsky3
 *
 * Created on August 1, 2017, 1:03 PM
 */

//c lib
//Wei
#include <string>
#include <cstring>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <string.h>
//Wei end

#include "NEST.hh"
#include "TestSpectra.hh"
#include "analysis.hh"

#include "Detectors/DetectorExample_XENON10.hh"
#include "Detectors/LUX_Run3.hh"
#include "Detectors/LUX_Run4.hh"

using namespace std;
using namespace NEST;

/*
 * 
 */
bool weihackgamma=1;

double band[200][6], energies[3];
vector<vector<double>> GetBand ( vector<double> S1s, vector<double> S2s, bool resol );
void GetEnergyRes ( vector<double> Es );


int main ( int argc, char** argv ) {
  
  // Instantiate your own VDetector class here, then load into NEST class constructor
  //DetectorExample_XENON10* detector = new DetectorExample_XENON10();
  //LUX_Run3* detector = new LUX_Run3();
  LUX_Run4* detector = new LUX_Run4();
	
  // Custom parameter modification functions
  //detector->ExampleFunction();

  // Run 3 (only uncomment if you need to change from default -> calibration)
  //detector->SetParameters_CH3T();
  //detector->SetParameters_DD();
  // Run 4 (will need to choose a time bin)
  detector->SetParameters_TimeBin1();
  //detector->SetParameters_TimeBin2();
  //detector->SetParameters_TimeBin3();
  //detector->SetParameters_TimeBin4();

  // Construct NEST class using detector object
  NEST::NESTcalc n(detector);

  vector<double> signal1,signal2,signalE, vTable, NuisParam={1.,1.}; int index;
  string position, delimiter, token; size_t loc;
  double g2,pos_x,pos_y,pos_z,r,phi,driftTime, field, vD,vD_middle, atomNum=0,massNum=0, keVee=0.0;
  NEST::YieldResult yieldsMax;
  
  if (argc < 7)
    {
      cout << "This program takes 6 (or 7) inputs, with Z position in mm from bottom of detector:" << endl;
      cout << "\t./testNEST numEvts type_interaction E_min[keV] E_max[keV] field_drift[V/cm] x,y,z-position[mm] {optional:seed}" << endl << endl;
      cout << "For 8B, numEvts is kg-days of exposure with everything else same. For WIMPs:" << endl;
      cout << "\t./testNEST exposure[kg-days] {WIMP} m[GeV] x-sect[cm^2] field_drift[V/cm] x,y,z-position[mm] {optional:seed}" << endl << endl;
      cout << "For cosmic-ray muons or other similar particles with elongated track lengths:" << endl;
      cout << "\t./testNEST numEvts {MIP} LET[MeV*cm^2/gram] x,y-position[mm](Initial) field_drift[V/cm] x,y,z-position[mm](Final) {optional:seed}" << endl << endl;
      return 0;
    }
  unsigned long int numEvts = atoi(argv[1]);
  
  string type = argv[2];
  INTERACTION_TYPE type_num;
  INTERACTION_TYPE type_num_def; //Wei
  TestSpectra spec;
  if ( type == "NR" || type == "neutron" ) type_num = NR;
  else if (type == "WIMP")
    {
      if (atof(argv[3])<0.44) { cerr << "WIMP mass too low, you're crazy!" << endl; return 0; }
      type_num = WIMP;
      spec.wimp_spectrum_prep= spec.WIMP_prep_spectrum(atof(argv[3]), E_step);
      numEvts = RandomGen::rndm()->poisson_draw(spec.wimp_spectrum_prep.integral * 1.0 * atof(argv[1]) * atof(argv[4]) / 1e-36);
    }
  else if ( type == "B8" || type == "Boron8" || type == "8Boron" || type == "8B" || type == "Boron-8" )
    {
      type_num = B8;
      numEvts = RandomGen::rndm()->poisson_draw(0.0026 * atof(argv[1]));
    } else if ( type == "DD" || type == "D-D" ) type_num = DD;
  else if ( type == "AmBe" ) type_num = AmBe;
  else if ( type == "Cf" || type == "Cf252" || type == "252Cf" || type == "Cf-252" ) type_num = Cf;
  else if ( type == "ion" || type == "nucleus" || type == "alpha" ) {
    type_num = ion;
    if ( type == "alpha" ) {
      atomNum = 2; massNum = 4;
    }
    else {
      cerr << "Atomic Number: "; cin >> atomNum;
      cerr << "Mass Number: "; cin >> massNum;
    } if ( atomNum == ATOM_NUM ) type_num = NR;
  }
  else if ( type == "gamma" || type == "gammaRay" ||
	    type == "x-ray" || type == "xray" || type == "xRay" || type == "X-ray" || type == "Xray" || type == "XRay" )
    type_num = gammaRay; //includes photo-absorption and electron capture
  else if ( type == "Kr83m" || type == "83mKr" || type == "Kr83" ) type_num = Kr83m;
  else if ( type == "CH3T" || type == "tritium" ) type_num = CH3T;
  else if ( type == "beta" || type == "ER" || type == "Compton" || type == "compton" || type == "electron" || type == "e-" ||
	    type == "muon" || type == "MIP" || type == "LIP" || type == "mu" || type == "mu-" )
    type_num = beta; //default electron recoil model
  //Wei
  else if (type=="weighted") type_num=weighted;
  //Wei end
  else {
    cerr << "UNRECOGNIZED PARTICLE TYPE!! VALID OPTIONS ARE:" << endl;
    cerr << "NR or neutron," << endl;
    cerr << "WIMP," << endl;
    cerr << "B8 or Boron8 or 8Boron or 8B or Boron-8," << endl;
    cerr << "DD or D-D," << endl;
    cerr << "AmBe," << endl;
    cerr << "Cf or Cf252 or 252Cf or Cf-252," << endl;
    cerr << "ion or nucleus," << endl;
    cerr << "alpha," << endl;
    cerr << "gamma or gammaRay," << endl;
    cerr << "x-ray or xray or xRay or X-ray or Xray or XRay," << endl;
    cerr << "Kr83m or 83mKr or Kr83," << endl;
    cerr << "CH3T or tritium, and" << endl;
    cerr << "beta or ER or Compton or compton or electron or e-" << endl;
    cerr << "muon or MIP or LIP or mu or mu-" << endl;
    return 0;
  }
  
  double eMin = atof(argv[3]);
  double eMax = atof(argv[4]);
  
  double rho = n.SetDensity ( detector->get_T_Kelvin(), detector->get_p_bar() ); //cout.precision(12);
  if ( rho < 1. ) detector->set_inGas(true);
  
  // Print g1 and g2 values, requiring S2 calculation for 1 SE and x,y,d = 0,0,0
  vector<double> secondary = n.GetS2 ( 1, 0., 0., 0., 1. );
  
  if ( atof(argv[5]) == -1. ) {
    vTable = n.SetDriftVelocity_NonUniform(rho, z_step);
    vD_middle = vTable[int(floor(.5*detector->get_TopDrift()/z_step))];
  }
  
  //Wei
  else if ( atof(argv[5]) == -2. ) {
    vTable = n.SetDriftVelocity_NonUniform(rho, z_step);
    vD_middle = vTable[int(floor(.5*detector->get_TopDrift()/z_step))];
  }
  //Wei end
  
  else vD_middle = n.SetDriftVelocity(detector->get_T_Kelvin(), rho, atof(argv[5]));
  cout << "Density = " << rho << " g/mL" << "\t";
  cout << "central vDrift = " << vD_middle << " mm/us\n";
  cout << "\t\t\t\t\t\t\t\t\t\tNegative numbers are flagging things below threshold!\n";
  
  if ( type_num == Kr83m && eMin == 9.4 && eMax == 9.4 )
    fprintf(stdout, "t [ns]\t\tE [keV]\t\tfield [V/cm]\ttDrift [us]\tX,Y,Z [mm]\tNph\tNe-\tS1_raw [PE]\tS1_Zcorr\tS1c_spike\tNe-Extr\tS2_rawArea\tS2_Zcorr [phd]\n");
  else
    fprintf(stdout, "E [keV]\t\tfield [V/cm]\ttDrift [us]\tX,Y,Z [mm]\tNph\tNe-\tS1_raw [PE]\tS1_Zcorr\tS1c_spike\tNe-Extr\tS2_rawArea\tS2_Zcorr [phd]\n");
  
  if (argc >= 8) RandomGen::rndm()->SetSeed(atoi(argv[7]));
  
  //Wei
  string ERfile=""; //file that stored jpm results for data points, format in "xin, yin, zin, s2x, s2y, drift, nInterp ,distInterp,efield"
  double xin, yin, zin, s2x, s2y, drift, nInterp ,distInterp,efield=1.;
  std::string line;
  std::string readline;	
  ifstream infile;
  int iLine=-1,iEvent=0;
  int ner=-1;
  if (atof(argv[5]) == -2.) {
    ERfile=argv[8];  
    infile.open(ERfile.c_str());
    cerr<<"file name: "<<ERfile.c_str()<<"\n";
    if(!infile){
      cerr<<"Invalid Input File!!!!!\n";
    }
  }
  
  //Wei end
  
  if ( type_num == WIMP ) {
    yieldsMax = n.GetYields(      NR, 25.0, rho, detector->FitEF(0., 0., detector->get_TopDrift()/2.),
				  double(massNum), double(atomNum), NuisParam);
  }
  else if ( type_num == B8 ) {
    yieldsMax = n.GetYields(      NR, 4.00, rho, detector->FitEF(0., 0., detector->get_TopDrift()/2.),
				  double(massNum), double(atomNum), NuisParam);
  }
  //Wei
  else if (type_num == weighted){
    type_num_def=beta;
    yieldsMax = n.GetYields(type_num_def, eMax, rho, detector->FitEF(0., 0., detector->get_TopDrift()/2.),
			    double(massNum), double(atomNum), NuisParam);
  }
  //Wei end
  else {
    yieldsMax = n.GetYields(type_num, eMax, rho, detector->FitEF(0., 0., detector->get_TopDrift()/2.),
			    double(massNum), double(atomNum), NuisParam);
  }
  if ( (detector->get_g1()*yieldsMax.PhotonYield) > (2.*maxS1) && eMin != eMax )
    cerr << "\nWARNING: Your energy maximum may be too high given your maxS1.\n";
  
  double keV = -999.;
  for (unsigned long int j = 0; j < numEvts; j++) {
    
    if (eMin == eMax && eMin >= 0. && eMax > 0. ) {
      keV = eMin;
    } else {
      switch (type_num) {
      case CH3T:
	keV = spec.CH3T_spectrum(eMin, eMax);
	break;
      case B8: //normalize this to ~3500 / 10-ton / year, for E-threshold of 0.5 keVnr, OR 180 evts/t/yr/keV at 1 keV
	keV = spec.B8_spectrum(eMin, eMax);
	break;
      case AmBe: //for ZEPLIN-III FSR from HA (Pal '98)
	keV = spec.AmBe_spectrum(eMin, eMax);
	break;
      case Cf:
	keV = spec.Cf_spectrum(eMin, eMax);
	break;
      case DD:
	keV = spec.DD_spectrum(eMin, eMax);
	break;
      case WIMP:
	{
          keV = spec.WIMP_spectrum(spec.wimp_spectrum_prep, atof(argv[3]));
	}
	break;
      default:
	keV = eMin + (eMax - eMin) * RandomGen::rndm()->rand_uniform();
	break;
      }
    }
    
    if ( type_num != WIMP && type_num != B8 ) {
      if (keV > eMax) keV = eMax;
      if (keV < eMin) keV = eMin;
    }
    
  Z_NEW:
    //Wei
    position="0";
    if (atof(argv[5]) == -2. ){
      if(infile.eof()){cerr<<"end of file.\n"; infile.close();}
      iLine++;
      ner++; 
      if( iLine % 1000 == 0 ) {cerr<<"Reading line "<<iLine<<endl;}
      std::getline(infile, readline);
      std::istringstream iss(readline);
      if(!(iss>>xin>> yin>> zin>> s2x>> s2y>> drift>> nInterp >>distInterp>>efield)){
	iLine++; cerr<<"error/end: "<<endl; std::getline(infile, readline); cerr<<readline<<endl; continue;}	
    driftTime=drift;  
   
    }
    if (efield<=0) {goto Z_NEW;}
    //Wei end
    if ( atof(argv[6]) == -1. ) { // -1 means default, random location mode
      pos_z = 0. + ( detector->get_TopDrift() - 0. ) * RandomGen::rndm()->rand_uniform(); // initial guess
      r = detector->get_radius() * sqrt ( RandomGen::rndm()->rand_uniform() );
      phi = 2.*M_PI*RandomGen::rndm()->rand_uniform();
      pos_x = r * cos(phi); pos_y = r * sin(phi);
    }
    //Wei
    else if ( atof(argv[6]) == -2. ){
      pos_x=xin*10.; pos_y=yin*10.; pos_z=zin*10.;
      r = pow(pow(pos_x,2.)+pow(pos_y, 2.),0.5);
      phi=atan2(pos_y, pos_x);
    }
    //Wei end
    else {
      position = argv[6];
      delimiter = ",";
      loc = 0; int i = 0;
      while ( (loc = position.find(delimiter)) != string::npos ) {
	token = position.substr(0,loc);
	if ( i == 0 ) pos_x = stof(token);
	else pos_y = stof(token);
	position.erase(0,loc+delimiter.length());
	i++;
      }
      pos_z = stof(position);
      if ( stof(position) == -1. )
	pos_z = 0. + ( detector->get_TopDrift() - 0. ) * RandomGen::rndm()->rand_uniform();
      if ( stof(token) == -999. ) {
	r = detector->get_radius() * sqrt ( RandomGen::rndm()->rand_uniform() );
	phi = 2.*M_PI*RandomGen::rndm()->rand_uniform();
	pos_x = r * cos(phi); pos_y = r * sin(phi); }
    }
    
    if ( atof(argv[5]) == -1. ) { // -1 means use poly position dependence
      field = detector->FitEF(pos_x, pos_y, pos_z);
    }
    else if ( atof(argv[5]) == -2. ) { // -1 means use poly position dependence
      field = efield/100.;
    }
    else field = atof(argv[5]);
    
    if ( field <= 0. )
      cerr << "\nWARNING: A LITERAL ZERO FIELD MAY YIELD WEIRD RESULTS. USE A SMALL VALUE INSTEAD.\n";
    if ( field > 12.e3 )
      cerr << "\nWARNING: Your field is >12,000 V/cm. No data out here. Are you sure about this?\n";
    
    if ( atof(argv[5]) == -1. ) {
      //for ( int jj = 0; jj < vTable.size(); jj++ ) //DEBUG
      //cerr << double(jj)*z_step << "\t" << vTable[jj] << endl;
      index = int(floor(pos_z/z_step));
      vD = vTable[index];
    }
    //Wei
    else if ( atof(argv[5]) == -2. ) {
      //for ( int jj = 0; jj < vTable.size(); jj++ ) //DEBUG
      //cerr << double(jj)*z_step << "\t" << vTable[jj] << endl;
      index = int(floor(pos_z/z_step));
      vD = vTable[index];
    }
    //Wei end
    else
      vD = n.SetDriftVelocity(detector->get_T_Kelvin(),rho,field);
    driftTime = ( detector->get_TopDrift() - pos_z ) / vD; // (mm - mm) / (mm / us) = us
    if (atof(argv[5]) == -2. ) driftTime=drift;//Wei
    if ( ((atof(argv[5]) != -1.)||(atof(argv[5]) != -2.)) && detector->get_dt_min() > ( detector->get_TopDrift() - 0. ) / vD )//Wei
      //Wei   if ( atof(argv[5]) != -1. && detector->get_dt_min() > ( detector->get_TopDrift() - 0. ) / vD )
      { cerr << "ERROR: dt_min is too restrictive (too large)" << endl; return 0; }
    if ( (driftTime > detector->get_dt_max() || driftTime < detector->get_dt_min()) && (atof(argv[6]) == -1. || stof(position) == -1.) )
      goto Z_NEW;
    if ( detector->get_dt_max() > (detector->get_TopDrift()-0.)/vD && !j )
      { cerr << "WARNING: dt_max is greater than max possible" << endl; }
    
    // The following should never happen: this is simply a just-in-case code-block dealing with user error
    if ( pos_z <= 0. ) {
      cerr << "ERROR: unphysically low Z coordinate (vertical axis of detector) of " << pos_z << " mm" << endl;
      return 0;
    }
    if ( pos_z >= detector->get_TopDrift() || driftTime <= 0.0 ) {
      cerr << "ERROR: unphysically big Z coordinate (vertical axis of detector) of " << pos_z << " mm" << endl;
      return 0;
    }
    
    NEST::YieldResult yields; NEST::QuantaResult quanta;
    //Wei
    NEST::YieldResult yields_b; NEST::QuantaResult quanta_b;
    NEST::YieldResult yields_g; NEST::QuantaResult quanta_g;
    //Wei end
    if ( type == "muon" || type == "MIP" || type == "LIP" || type == "mu" || type == "mu-" ) {
      double xi = -999., yi = -999.;
      if ( atof(argv[4]) == -1. ) {
	r = detector->get_radius()
	  * sqrt ( RandomGen::rndm()->rand_uniform() );
	phi = 2.*M_PI*RandomGen::rndm()->rand_uniform();
	xi = r * cos(phi); yi = r * sin(phi);
      }
      else {
	position = argv[4];
	delimiter = ",";
	loc = 0; int ii = 0;
	while ( (loc = position.find(delimiter)) != string::npos ) {
	  token = position.substr(0,loc);
	  if ( ii == 0 ) xi = stof(token);
	  else yi = stof(token);
	  position.erase(0,loc+delimiter.length());
	  ii++;
	}
	yi = stof(position);
      }
      double dEOdx = eMin, eStep = dEOdx * rho * z_step * 1e2, refEnergy = 1e6; keV = 0.;
      int Nph = 0, Ne = 0;
      double xx = xi, yy = yi, zz = detector->get_TopDrift();
      double xf = pos_x; double yf = pos_y;
      double distance = sqrt(pow(xf-xi,2.)+pow(yf-yi,2.)+pow(detector->get_TopDrift(),2.));
      double norm[3];
      norm[0] = ( xf - xi ) / distance;
      norm[1] = ( yf - yi ) / distance;
      norm[2] = -detector->get_TopDrift() / distance;
      while ( zz > 0. && sqrt(pow(xx,2.)+pow(yy,2.)) < detector->get_radius() ) {
	yields = n.GetYields ( beta, refEnergy, rho, detector->FitEF ( xx, yy, zz ), double(massNum), double(atomNum), NuisParam );
	quanta = n.GetQuanta ( yields, rho );
	Nph+= quanta.photons * (eStep/refEnergy);
	index = int(floor(zz/z_step));
	if ( index >= vTable.size() )
	  index = vTable.size() - 1;
	vD = vTable[index];
	driftTime = ( detector->get_TopDrift() - zz ) / vD;
	if ( pos_z >= detector->get_cathode() )
//Wei	
if (atof(argv[5]) == -2. ) driftTime=drift;
  Ne += quanta.electrons*(eStep/refEnergy)*exp(-driftTime/detector->get_eLife_us());
//Wei end
	keV+= eStep;
	xx += norm[0] * z_step;
	yy += norm[1] * z_step;
	zz += norm[2] * z_step; //cout << xx << " " << yy << " " << zz << endl;
      }
      quanta.photons = Nph; quanta.electrons = Ne;
      pos_z = detector->get_TopDrift() / 2.; driftTime = 0.00;
      vD = vD_middle;
      pos_x = .5*(xi+xf); pos_y = .5*(yi+yf);
      field = detector->FitEF(pos_x,pos_y,pos_z);
    }
    else if (type_num ==weighted){
      double k1 = 1.06676409, k2 = 5.00368406;
      double wt = 0.5 + 0.5 * erf(k1 * (log(keV) - k2));
      yields_b = n.GetYields(beta, keV, rho, field, massNum, atomNum, NuisParam);
      quanta_b = n.GetQuanta(yields_b, rho);
      yields_g = n.GetYields(gammaRay, keV, rho, field, massNum,atomNum,NuisParam);
      quanta_g = n.GetQuanta(yields_g, rho);
      quanta.photons = int( (1 - wt) * quanta_b.photons + wt * quanta_g.photons );
      quanta.electrons = int( (1 - wt) * quanta_b.electrons + wt * quanta_g.electrons );
      yields.Lindhard = 1.;
    }
    else {
      yields = n.GetYields(type_num,keV,rho,field,
			   double(massNum),double(atomNum),NuisParam);
      quanta = n.GetQuanta(yields,rho);
    }
    
    if ( j == 0 ) g2 = 20.;
    double Nphd_S2 = g2 * quanta.electrons * exp(-driftTime/detector->get_eLife_us());
    if ( !MCtruthPos && Nphd_S2 > PHE_MIN ) {
      vector<double> xySmeared(2); xySmeared = n.xyResolution ( pos_x, pos_y, Nphd_S2 );
      pos_x = xySmeared[0];
      pos_y = xySmeared[1];
    }
    
    vector<double> scint = n.GetS1(quanta.photons,pos_x,pos_y,pos_z,
				   vD,vD_middle,type_num);
    if ( usePD == 0 && fabs(scint[3]) > minS1 && scint[3] < maxS1 )
      signal1.push_back(scint[3]);
    else if ( usePD == 1 && fabs(scint[5]) > minS1 && scint[5] < maxS1 )
      signal1.push_back(scint[5]);
    else if ( usePD >= 2 && fabs(scint[7]) > minS1 && scint[7] < maxS1 )
      signal1.push_back(scint[7]);
    else signal1.push_back(-999.);
    
    if ( pos_z < detector->get_cathode() ) quanta.electrons = 0;
    vector<double> scint2= n.GetS2(quanta.electrons, pos_x, pos_y, driftTime, vD);
    if ( usePD == 0 && fabs(scint2[5]) > minS2 && scint2[5] < maxS2 )
      signal2.push_back(scint2[5]);
    else if ( usePD >= 1 && fabs(scint2[7]) > minS2 && scint2[7] < maxS2 )
      signal2.push_back(scint2[7]); //no spike option for S2
    else signal2.push_back(-999.);

    g2 = fabs(scint2[8]);
    if ( !MCtruthE ) {
      double Nph, g1 = detector->get_g1(), Ne;
      if ( usePD == 0 )
	Nph= fabs(scint[3]) / (g1*(1.+detector->get_P_dphe()));
      else if ( usePD == 1 ) Nph = fabs(scint[5]) / g1;
      else Nph = fabs(scint[7]) / g1;
      if ( usePD == 0 )
	Ne = fabs(scint2[5]) / (g2*(1.+detector->get_P_dphe()));
      else Ne = fabs(scint2[7]) / g2;
      if ( signal1.back() <= 0. )
	Nph= 0.;
      if ( signal2.back() <= 0. )
	Ne = 0.;
      if ( yields.Lindhard > DBL_MIN && Nph > 0. && Ne > 0. ) {
	keV = ( Nph + Ne ) * W_DEFAULT * 1e-3 / yields.Lindhard;
	keVee+=( Nph + Ne ) * W_DEFAULT * 1e-3;
      }
      else
	keV = 0.;
    }
    if ( signal1.back() <= 0. || signal2.back() <= 0. )
      signalE.push_back(0.);
    else
      signalE.push_back(keV);
              
    if ( 1 ) { //fabs(scint[7]) > PHE_MIN && fabs(scint2[7]) > PHE_MIN ) { //if you want to skip specific below-threshold events, then please comment in this if statement
    if (atof(argv[5]) == -2. ) driftTime=drift;
      printf("%.6f\t%.6f\t%.6f\t%.0f, %.0f, %.0f\t%d\t%d\t",keV,field,driftTime,pos_x,pos_y,pos_z,quanta.photons,quanta.electrons); //comment this out when below line in
      //printf("%.6f\t%.6f\t%.6f\t%.0f, %.0f, %.0f\t%lf\t%lf\t",keV,field,driftTime,pos_x,pos_y,pos_z,yields.PhotonYield,yields.ElectronYield); //for when you want means
      //Wei if ( pos_z < detector->get_cathode() ) printf("g-X ");
      if(0||
	 //Wei      if ( keV > 1000. || scint[5] > maxS1 || scint2[7] > maxS2 ||
	 //switch to exponential notation to make output more readable, if energy is too high (>1 MeV)
	 type == "muon" || type == "MIP" || type == "LIP" || type == "mu" || type == "mu-" ) {
	printf("%e\t%e\t%e\t", scint[2], scint[5], scint[7]);
	printf("%li\t%e\t%e\n", (long)scint2[0], scint2[4], scint2[7]);
      }
      else {
	printf("%.6e\t%.6e\t%.6e\t", scint[2], scint[5], scint[7]); //see GetS1 inside of NEST.cpp for full explanation of all 8 scint return vector elements. Sample 3 most common
	//Wei	printf("%i\t%.6f\t%.6f\n", (int)scint2[0], scint2[4], scint2[7]); //see GetS2 inside of NEST.cpp for full explanation of all 8 scint2 vector elements. Change as you desire 
	printf("%i\t%.6e\t%.6e\t", (int)scint2[0], scint2[4], scint2[7]); //see GetS2 inside of NEST.cpp for full explanation of all 8 scint2 vector elements. Change as you desire //Wei
	if (atof(argv[5]) == -2. ){printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t",xin, yin, zin, s2x, s2y, drift, nInterp ,distInterp,efield);}
	//	cerr<<"test1: "<<xin<< yin<< zin<<endl;
	printf("\n");
      }
    }
    
  }
  
  if ( eMin != eMax ) {
    if ( useS2 == 2 )
      GetBand ( signal2, signal1, false );
    else
      GetBand ( signal1, signal2, false );
    fprintf(stderr,"Bin Center\tBin Actual\tHist Mean\tMean Error\tHist Sigma\t\tEff[%%>thr]\n");
    for ( int j = 0; j < numBins; j++ ) {
      fprintf(stderr,"%lf\t%lf\t%lf\t%lf\t%lf\t\t%lf\n",band[j][0],band[j][1],band[j][2],band[j][4],band[j][3],band[j][5]*100.);
      if ( band[j][0] <= 0.0 || band[j][1] <= 0.0 || band[j][2] <= 0.0 || band[j][3] <= 0.0 || band[j][4] <= 0.0 || band[j][5] <= 0.0 ||
           std::isnan(band[j][0]) || std::isnan(band[j][1]) || std::isnan(band[j][2]) || std::isnan(band[j][3]) || std::isnan(band[j][4]) || std::isnan(band[j][5]) )
	{ if ( eMax != -999. ) {
	    if( ( (detector->get_g1()*yieldsMax.PhotonYield) < maxS1 || (g2*yieldsMax.ElectronYield) < maxS2 ) && j != 0)
	      cerr << "WARNING: Insufficient number of high-energy events to populate highest bins is likely.\n";
	    else
	      cerr << "WARNING: Insufficient number of low-energy events to populate lowest bins is likely. Increase minS1 and/or minS2.\n";
	  }
	  eMax = -999.; }
    }
  }
  else {
    GetBand ( signal1, signal2, true );
    GetEnergyRes ( signalE );
    if ( type_num == NR ) {
      fprintf(stderr,"S1 Mean\t\tS1 Res [%%]\tS2 Mean\t\tS2 Res [%%]\tEc [keVnr]\tEc Res[%%]\tEff[%%>thr]\tEc [keVee]\n");
      keVee /= numEvts;
    }
    else
      fprintf(stderr,"S1 Mean\t\tS1 Res [%%]\tS2 Mean\t\tS2 Res [%%]\tEc Mean\t\tEc Res[%%]\tEff[%%>thr]\n"); //the C here refers to the combined (S1+S2) energy scale
    for ( int j = 0; j < numBins; j++ ) {
      fprintf(stderr,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t",band[j][0],band[j][1]/band[j][0]*100.,
	      band[j][2],band[j][3]/band[j][2]*100.,energies[0],energies[1]/energies[0]*100.,energies[2]*100.);
      if ( type_num == NR ) fprintf(stderr,"%lf\n",keVee/energies[2]); else fprintf(stderr,"\n");
      if ( band[j][0] <= 0.0 || band[j][1] <= 0.0 || band[j][2] <= 0.0 || band[j][3] <= 0.0 ||
	   std::isnan(band[j][0]) || std::isnan(band[j][1]) || std::isnan(band[j][2]) || std::isnan(band[j][3]) )
	cerr << "CAUTION: YOUR S1 and/or S2 MIN and/or MAX may be set to be too restrictive, please check.\n";
      else if ( energies[0] == eMin || energies[0] == eMax || energies[1] <= 0.0 )
	cerr << "If your energy resolution is 0% then you probably still have MC truth energy on." << endl;
      else ;
    }
  }
  
  return 1;
  
}

vector<vector<double>> GetBand ( vector<double> S1s,
				 vector<double> S2s, bool resol ) {
  
  vector<vector<double>> signals;
  signals.resize(numBins,vector<double>(1,-999.));
  double binWidth, border;
  if ( useS2 == 2 ) {
    binWidth = ( maxS2 - minS2 ) / double(numBins);
    border = minS2;
  }
  else {
    binWidth = ( maxS1 - minS1 ) / double(numBins);
    border = minS1;
  }
  int i = 0, j = 0; double s1c, numPts;
  unsigned long reject[200] = {0};
  
  if ( resol ) {
    numBins = 1;
    binWidth = DBL_MAX;
  }
  
  for ( i = 0; i < S1s.size(); i++ ) {
    for ( j = 0; j < numBins; j++ ) {
      s1c = border + binWidth/2. + double(j) * binWidth;
      if ( i == 0 && !resol ) band[j][0] = s1c;
      if ( fabs(S1s[i]) > (s1c-binWidth/2.) && fabs(S1s[i]) <= (s1c+binWidth/2.) ) {
	if ( S1s[i] >= 0. && S2s[i] >= 0. ) {
	  if ( resol ) {
	    signals[j].push_back(S2s[i]);
	  }
	  else {
	    if ( useS2 == 0 )
	      { if ( S1s[i] && S2s[i] ) signals[j].push_back(log10(S2s[i]/S1s[i])); else signals[j].push_back(0.); }
	    else if ( useS2 == 1 )
	      { if ( S1s[i] && S2s[i] ) signals[j].push_back(log10(S2s[i])); else signals[j].push_back(0.); }
	    else
	      { if ( S1s[i] && S2s[i] ) signals[j].push_back(log10(S1s[i]/S2s[i])); else signals[j].push_back(0.); }
	  }
	  band[j][2] += signals[j].back();
	  if ( resol )
	    band[j][0] += S1s[i];
	  else
	    band[j][1] += S1s[i];
	}
	else
	  reject[j]++;
	break; }
    }
  }
  
  for ( j = 0; j < numBins; j++ ) {
    if ( band[j][0] <= 0. && !resol ) band[j][0] = border + binWidth/2. + double(j) * binWidth;
    signals[j].erase(signals[j].begin());
    numPts = (double)signals[j].size();
    if (resol)
      band[j][0] /= numPts;
    band[j][1] /= numPts;
    band[j][2] /= numPts;
    for ( i = 0; i < (int)numPts; i++ ) {
      if ( signals[j][i] != -999. ) band[j][3] += pow(signals[j][i]-band[j][2],2.);
      if ( resol && S1s[i] >= 0.0 ) band[j][1] += pow(S1s[i]-band[j][0],2.); //std dev calc
    }
    band[j][3] /= numPts - 1.;
    band[j][3] = sqrt(band[j][3]);
    if ( resol ) {
      band[j][1] /= numPts - 1.;
      band[j][1] = sqrt(band[j][1]);
    }
    band[j][4] = band[j][3] / sqrt ( numPts );
    band[j][5] = numPts/
      (numPts+double(reject[j]));
  }
  
  return signals;
  
}

void GetEnergyRes ( vector<double> Es ) {
  
  int i, numPts = Es.size();
  double numerator = 0.;
  
  for ( i = 0; i < numPts; i++ ) {
    if ( Es[i] > 0. )
      { energies[0] += Es[i]; numerator++; }
  }
  
  energies[0] /= numerator;
  
  for ( i = 0; i < numPts; i++ ) {
    if ( Es[i] > 0. )
      energies[1] += pow(energies[0]-Es[i],2.);
  }
  
  energies[1] /= numerator - 1.;
  energies[1] = sqrt(energies[1]);
  
  energies[2] = numerator / double ( numPts );
  return;
  
}
