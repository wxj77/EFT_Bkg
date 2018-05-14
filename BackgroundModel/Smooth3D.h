// Algorithm for smoothing 3D histograms, extended from ROOT
// method Smooth() for 2D hists.  Made specifically for smoothing
// (r, phi, drift) 3D hists, where phi is treated as having periodic
// boundaries.

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

void Smooth3D( TH3F* histo ){
   
   Double_t k5b[5][5] =  { { 0, 1, 2, 1, 0 },
                           { 1, 2, 4, 2, 1 },
                           { 2, 4, 8, 4, 2 },
                           { 1, 2, 4, 2, 1 },
                           { 0, 1, 2, 1, 0 } };
   
   Double_t k3a[3][3][3] = {
                                { { 0, 0, 0 },
                                  { 0, 1, 0 },
                                  { 0, 0, 0 } },
                                { { 0, 1, 0 },
                                  { 1, 2, 1 },
                                  { 0, 1, 0 } },
                                { { 0, 0, 0 },
                                  { 0, 1, 0 },
                                  { 0, 0, 0 } }
                            };
   
   //Int_t ksize_x=5;
   //Int_t ksize_y=5;
   //Double_t *kernel = &k5b[0][0];
   
   Int_t ksize_x=3;
   Int_t ksize_y=3;
   Int_t ksize_z=3;
   Double_t *kernel = &k3a[0][0][0];
   
   // find i,j ranges
   Int_t ifirst = histo->GetXaxis()->GetFirst();
   Int_t ilast  = histo->GetXaxis()->GetLast();
   Int_t jfirst = histo->GetYaxis()->GetFirst();
   Int_t jlast  = histo->GetYaxis()->GetLast();
   Int_t kfirst = histo->GetZaxis()->GetFirst();
   Int_t klast  = histo->GetZaxis()->GetLast();
   
   // Determine the size of the bin buffer(s) needed
   Double_t nentries = histo->GetEntries();
   Int_t nx = histo->GetNbinsX();
   Int_t ny = histo->GetNbinsY();
   Int_t nz = histo->GetNbinsZ();
   Int_t bufSize  = (nx+2)*(ny+2)*(nz+2);
   Double_t *buf  = new Double_t[bufSize];
   Double_t *ebuf = 0;
  // if (fSumw2.fN) ebuf = new Double_t[bufSize];

   // Copy all the data to the temporary buffers
   Int_t i,j,k,bin;
   for (i=ifirst; i<=ilast; i++){
      for (j=jfirst; j<=jlast; j++){
         for (k=kfirst; k<=klast; k++){
            bin = histo->GetBin(i,j,k);
            buf[bin] = histo->GetBinContent(bin)/histo->GetYaxis()->GetBinWidth(j);
            //if (ebuf) ebuf[bin]=GetBinError(bin);
         }
      }
   }

   // Kernel tail sizes (kernel sizes must be odd for this to work!)
   Int_t x_push = (ksize_x-1)/2;
   Int_t y_push = (ksize_y-1)/2;
   Int_t z_push = (ksize_z-1)/2;

   // main work loop
   //Loop over each bin
   for (i=ifirst; i<=ilast; i++){
      for (j=jfirst; j<=jlast; j++) {
         for (k=kfirst; k<=klast; k++) {
         
            Double_t content = 0.0;
            Double_t error = 0.0;
            Double_t norm = 0.0;
            
            //Loop over bins within the 
            //kernel tail sizes around the current bin
            for (Int_t n=0; n<ksize_x; n++) {
               for (Int_t m=0; m<ksize_y; m++) {
                  for (Int_t p=0; p<ksize_z; p++) {
                  
                     Int_t xb = i+(n-x_push);
                     Int_t yb = j+(m-y_push);
                     Int_t zb = k+(p-z_push);
                     
                     /*cout<< "xb = " << xb <<endl;
                     cout<< "yb = " << yb <<endl;
                     cout<< "zb = " << zb <<endl;
                     cout<<endl;*/
                    // if ( (xb >= 1) && (xb <= nx) && (yb >= 1) && (yb <= ny) && (zb >= 1) && (zb <= nz) ) { //check that you're in range still...
                     
                     if ( (xb >= 1) && (xb <= nx) && (zb >= 1) && (zb <= nz) ) { //check that r and drift are in range
                        //Make y circular
                        if( yb < 1 ){
                           //cout<<"yb = "<<yb<<" and now yb = ";
                           yb = ny - TMath::Abs( 1 - yb ) - 1;
                           //cout<<yb<<endl;
                           //cout<<endl;
                        }
                        if( yb > ny ){
                           
                           yb = TMath::Abs( ny - yb );
                        }
                       
                        bin = histo->GetBin(xb,yb,zb);
                        Double_t k2 = kernel[n*ksize_y*ksize_x + m*ksize_y + p];
                        
                        if ( k2 != 0.0 ) {
                           norm    += k2;
                           content += k2*buf[bin];
                        }
                        
                     } //end if in range
                     
                  }
               }
            }
            //done looping around kernel tails
            
            if ( norm != 0.0 ) histo->SetBinContent(i,j,k,(content/norm)*histo->GetYaxis()->GetBinWidth(j) );
            
         }
      }
   }
   histo->SetEntries( nentries );
   
   delete [] buf;
   delete [] ebuf;
   
}
