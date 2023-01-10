#include "FPplots.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "EnergyLoss.h"
#include "MassLookup.h"
#include <iostream>
#include <TLorentzVector.h>
#include <vector>

/*
RMS 8/3/2022
converting channels from focal plane detector (xavg) to excitation energies (Ex)
generate preliminary result plots with calibrated focal plane energies on x-axis
*/

void focalplane::RunLiFPplots(){
//Experimental Constants
	double malpha = MASS.FindMass(2,4);
	double m3He = MASS.FindMass(2,3);
	double m7Li = MASS.FindMass(3,7);
	double m6Li = MASS.FindMass(3,6);

	double C = 299792458; 
    double QBRHO2P = 1.0E-9*C; //converts QBrho to momentum (cm*kG -> MeV)	
	double beam = 7.5; //MeV
	double spsTheta = 20*TMath::Pi()/180; //radians 
	double B = 7.30142; //kG
	double av1, av2, av3, Qvalue; //av = arbitrary variables
	av1=1+(malpha/m6Li);
	av2=(m3He/m6Li)-1;
	av3=2*TMath::Sqrt(malpha*m3He)/m6Li;
//creating plots and file to save them to
	TFile *plotsfile = new TFile ("/Users/rmshaffer/RMSRecon_v1/plots/Li6_FPplots.root","recreate");

	TH1F *xavg_1par = new TH1F("xavg_1par","6Li_1par",500,-1,7.5);
	TH1F *xavg_2par = new TH1F("xavg_2par","6Li_2par",500,-1,7.5);

	TH2F *sabre_1par = new TH2F("sabreE_FPE_1par","6Li_1par",500,-1,7.5,500,0,10000);
	TH2F *sabre_2par = new TH2F("sabreE_FPE_2par","6Li_2par",500,-1,7.5,500,0,10000);

	double_t sabreE_1par;
	double_t theta_1par;
	double_t phi_1par;
	double_t xavg1;

	double_t E1_2par;
	double_t theta1_2par;
	double_t phi1_2par;	
	double_t E2_2par;
	double_t theta2_2par;
	double_t phi2_2par;
	double_t xavg2;



//1 decay particle detected
std::ifstream datafile_1par;
datafile_1par.open("./etc/LiF_1par.txt");
while(datafile_1par >> sabreE_1par >> theta_1par >> phi_1par >> xavg1){
	double_t v1par[4] = {sabreE_1par,theta_1par,phi_1par,xavg1};

	double rho = 79.627+0.03741*v1par[3]; //focal plane conversion equation from GWM SabreRecon (channels -> rho[cm])
	double num = 2*B*rho*QBRHO2P;
	double Ealpha = (num*num)/(2*malpha);
	Qvalue = av1*Ealpha + av2*beam - av3*TMath::Cos(spsTheta)*TMath::Sqrt(Ealpha*beam);

	double Qgs = m3He + m7Li - malpha - m6Li;
	double Ex = Qgs - Qvalue;  

	xavg_1par->Fill(Ex);
	sabre_1par->Fill(Ex,v1par[0]); 
}
datafile_1par.close();

std::ifstream datafile_2par;
datafile_2par.open("./etc/LiF_2par.txt");
while(datafile_2par >> E1_2par >> theta1_2par >> phi1_2par >> E2_2par >> theta2_2par >> phi2_2par >> xavg2){
	double_t v2par[7] = {E1_2par,theta1_2par,phi1_2par,E2_2par,theta2_2par,phi2_2par,xavg2};

	double rho = 79.627+0.03741*v2par[6]; //focal plane conversion equation from GWM SabreRecon (channels -> rho[cm])
	double num = 2*B*rho*QBRHO2P;
	double Ealpha = (num*num)/(2*malpha);
	Qvalue = av1*Ealpha + av2*beam - av3*TMath::Cos(spsTheta)*TMath::Sqrt(Ealpha*beam);

	double Qgs = m3He + m7Li - malpha - m6Li;
	double Ex = Qgs - Qvalue;  

	xavg_2par->Fill(Ex);
	sabre_2par->Fill(Ex,v2par[0]+v2par[3]); 
}

plotsfile->cd();
xavg_1par->Write();
xavg_2par->Write();
sabre_1par->Write();
sabre_2par->Write();
plotsfile->Close();
}

//FIX YO SHIT. your FP calibration is trash and artifically adjusted. 

void focalplane::RunBorFPplots(){
//Experimental Constants
	double malpha = MASS.FindMass(2,4);
	double m3He = MASS.FindMass(2,3);
	double m10B = MASS.FindMass(5,10);
	double m9B = MASS.FindMass(5,9);

	double C = 299792458; 
    double QBRHO2P = 1.0E-9*C; //converts QBrho to momentum (cm*kG -> MeV)	
	double beam = 7.5; //MeV
	double spsTheta = 20*TMath::Pi()/180; //radians 
	double B = 7.30142; //kG
	double av1, av2, av3, Qvalue; //av = arbitrary variables
	av1=1+(malpha/m9B);
	av2=(m3He/m9B)-1;
	av3=2*TMath::Sqrt(malpha*m3He)/m9B;
//creating plots and file to save them to
	TFile *plotsfile = new TFile ("/Users/rmshaffer/RMSRecon_v1/plots/9B_FPplots.root","recreate");

	TH1F *xavg_1par = new TH1F("xavg_1par","9B_1par",500,-1,7.5);
	TH1F *xavg_2par = new TH1F("xavg_2par","9B_2par",500,-1,7.5);
	TH1F *xavg_3par = new TH1F("xavg_3par","9B_3par",500,-1,7.5);

	TH2F *sabre_1par = new TH2F("sabreE_FPE_1par","9B_1par",500,-1,7.5,500,0,10000);
	TH2F *sabre_2par = new TH2F("sabreE_FPE_2par","9B_2par",500,-1,7.5,500,0,10000);
	TH2F *sabre_3par = new TH2F("sabreE_FPE_3par","9B_3par",500,-1,7.5,500,0,10000);

	double_t sabreE_1par, theta_1par, phi_1par, xavg1;

	double_t E1_2par,theta1_2par,phi1_2par;
	double_t E2_2par,theta2_2par,phi2_2par;
	double_t xavg2;

	double_t E1_3par,theta1_3par,phi1_3par;
	double_t E2_3par,theta2_3par,phi2_3par;
	double_t E3_3par,theta3_3par,phi3_3par;
	double_t xavg3;

//1 decay particle detected
std::ifstream datafile_1par;
datafile_1par.open("./etc/9B_1par.txt");
while(datafile_1par >> sabreE_1par >> theta_1par >> phi_1par >> xavg1){
	double_t v1par[4] = {sabreE_1par,theta_1par,phi_1par,xavg1};

	double rho = 74.465+0.0371*v1par[3]+(2.0E-6*v1par[3]*v1par[3]); //focal plane conversion equation from GWM SabreRecon (channels -> rho[cm])
	double num = 2*B*rho*QBRHO2P;
	double Ealpha = (num*num)/(2*malpha);
	Qvalue = av1*Ealpha + av2*beam - av3*TMath::Cos(spsTheta)*TMath::Sqrt(Ealpha*beam);

	double Qgs = m3He + m10B - malpha - m9B;
	double Ex = Qgs - Qvalue;  
double shiftedE = Ex-2.2;
	xavg_1par->Fill(shiftedE);
	sabre_1par->Fill(shiftedE,v1par[0]); 
}
datafile_1par.close();

//2 decay particles detected
std::ifstream datafile_2par;
datafile_2par.open("./etc/9B_2par.txt");
while(datafile_2par >> E1_2par >> theta1_2par >> phi1_2par >> E2_2par >> theta2_2par >> phi2_2par >> xavg2){
	double_t v2par[7] = {E1_2par,theta1_2par,phi1_2par,E2_2par,theta2_2par,phi2_2par,xavg2};

	double rho = 74.465+0.0371*v2par[6]+(2.0E-6*v2par[6]*v2par[6]); //focal plane conversion equation from GWM SabreRecon (channels -> rho[cm])
	double num = 2*B*rho*QBRHO2P;
	double Ealpha = (num*num)/(2*malpha);
	Qvalue = av1*Ealpha + av2*beam - av3*TMath::Cos(spsTheta)*TMath::Sqrt(Ealpha*beam);

	double Qgs = m3He + m10B - malpha - m9B;
	double Ex = Qgs - Qvalue;  
double shiftedE = Ex-2.2;
	xavg_2par->Fill(shiftedE);
	sabre_2par->Fill(shiftedE,v2par[0]+v2par[3]); 
}

//3 decay particles detected
std::ifstream datafile_3par;
datafile_3par.open("./etc/9B_3par.txt");
while(datafile_3par >> E1_3par >> theta1_3par >> phi1_3par >> E2_3par >> theta2_3par >> phi2_3par >> 
					E3_3par >> theta3_3par >> phi3_3par >>xavg3){
	double_t v3par[10] = {E1_3par,theta1_3par,phi1_3par,E2_3par,theta2_3par,phi2_3par,E3_3par,theta3_3par,phi3_3par,xavg3};

	double rho = 74.465+0.0371*v3par[9]+(2.0E-6*v3par[9]*v3par[9]); //focal plane conversion equation from GWM SabreRecon (channels -> rho[cm])
	double num = 2*B*rho*QBRHO2P;
	double Ealpha = (num*num)/(2*malpha);
	Qvalue = av1*Ealpha + av2*beam - av3*TMath::Cos(spsTheta)*TMath::Sqrt(Ealpha*beam);

	double Qgs = m3He + m10B - malpha - m9B;
	double Ex = Qgs - Qvalue;  
double shiftedE = Ex-2.2;
	xavg_3par->Fill(shiftedE);
	sabre_3par->Fill(shiftedE,v3par[0]+v3par[3]+v3par[6]); 
}



plotsfile->cd();
xavg_1par->Write();
xavg_2par->Write();
xavg_3par->Write();
sabre_1par->Write();
sabre_2par->Write();
sabre_3par->Write();
plotsfile->Close();
}


