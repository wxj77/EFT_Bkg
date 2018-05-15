#ifndef IsoS1S2Spectrum
#define IsoS1S2Spectrum
#include <iostream>
#include <math.h>
#include <cmath>


double fitf(double *v, double *par=NULL){
  double fitval;
  fitval= pow(10., par[0]+par[1]*log10(v[0]));
  return fitval;
}


double isoS1(double *v, double *par=NULL){//Hz/phe
   double fitval=0.; 
   double x[1]={v[0]};
   
    double ppar[2]={-1.944, -1.803};
   double value[9] ={0, 0.037673065071 ,
0.00213345029058 ,
0.00725005039012 ,
0.00280317624699 ,
0.000681294777939 ,
0.000539872123915 ,
0.000316707912248 ,
0.000262137373865  };
/*   
double par[2]={-.165, -1.803};
   double value[9] ={0, 2.26038390426 ,
0.128007017435 ,
0.435003023407 ,
0.168190574819 ,
0.0408776866764 ,
0.0323923274349 ,
0.0190024747349 ,
0.0157282424319 };
*/

   if(v[0]>9) {fitval= fitf(x, ppar);}
   else if (v[0]<1) {fitval=value[0];}
   else if ((1<=v[0]) && (v[0]<2)) {fitval=value[1];}
   else if ((2<=v[0]) && (v[0]<3)) {fitval=value[2];}
   else if ((3<=v[0]) && (v[0]<4)) {fitval=value[3];}
   else if ((4<=v[0]) && (v[0]<5)) {fitval=value[4];}  
   else if ((5<=v[0]) && (v[0]<6)) {fitval=value[5];}
   else if ((6<=v[0]) && (v[0]<7)) {fitval=value[6];}
   else if ((7<=v[0]) && (v[0]<8)) {fitval=value[7];}
   else if ((8<=v[0]) && (v[0]<9)) {fitval=value[8];}  
         else {fitval=0;}   
   return fitval;
   
}


double isoS2(double* v, double *par=NULL){//Hz/phe
   double fitval=0.; 
   double x[1]={v[0]};
   
    double ppar[2]={-4.310, -0.883};
    if(v[0]>100) {fitval= fitf(x, ppar);}
 
  // else if (((100<=v[0]) && v[0]<700)) {fitval=1.e-3;}   
      else if (v[0]<100) {fitval=0;}
      else {fitval=0;}
   return fitval;
   
}


#endif

