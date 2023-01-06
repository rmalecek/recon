#include "boron.h"
#include "TH1F.h"
#include "TFile.h"
#include "EnergyLoss.h"
#include "MassLookup.h"
#include <iostream>
#include <TLorentzVector.h>
#include <vector>

/*
By RMS 7/21/22
Invariant mass and missing mass calculations for B9 done with 
2 and 3 particle text files output from printData Plotter in GWMEVB code

MMM possibilities: AA, AP, PA
MMM missing particle = target + beam - rxnalpha - detected particle
*/


void boron::RunBoronIMM(){
 	
    double mproton = MASS.FindMass(1,1);
    double m3He = MASS.FindMass(2,3);
    double malpha = MASS.FindMass(2,4);
    double m10B = MASS.FindMass(5,10);
    double m9B = MASS.FindMass(5,9);

//creates file plots will be written to
TFile *IMMplots = new TFile ("/Users/rmshaffer/RMSRecon/plots/B9IMMtest.root","recreate");

//creates plots to fill
TH1F *configAAP = new TH1F("AAPconfig","9B_IMMExEn",550,-1,10);
TH1F *configAPA = new TH1F("APAconfig","9B_IMMExEn",550,-1,10);
TH1F *configPAA = new TH1F("PAAconfig","9B_IMMExEn",550,-1,10);
TH1F *IMMexE = new TH1F("9B_3par","IMM_ExEnergy",550,-1,10);

	double_t E1, E2, E3;
	double_t theta1, theta2, theta3;
	double_t phi1, phi2, phi3;
	double_t xavg;
	int a1, a2, pro;

	double_t E[3];
	double_t p[3];

	double_t imm_exEn_A;
	double_t imm_exEn_P;

//IMM for 3 particle events
std::ifstream datafile_3par;
datafile_3par.open("./etc/9B_3par.txt");

//energies in keV and angles in degrees
while(datafile_3par >> E1 >> theta1 >> phi1 >> E2 >> theta2 >> phi2 >> E3 >> theta3 >> phi3 >> xavg){

TLorentzVector alpha_par1, alpha1_par2, alpha_par2, alpha2_par3, proton_par1, proton, B9_A, B9_P;
double_t KE[3]={E1/1000,E2/1000,E3/1000}; //convert energies to MeV
double_t theta[3]={theta1*(TMath::Pi()/180),theta2*(TMath::Pi()/180.0),theta3*(TMath::Pi()/180.0)}; //convert degrees to radians
double_t phi[3]={phi1*(TMath::Pi()/180.0),phi2*(TMath::Pi()/180.0),phi3*(TMath::Pi()/180.0)};

if(xavg>100 && xavg<110){ //ground state
//possible 3 particle configurations; AAP, APA, PAA
//first particle alpha1
for(a1=0; a1<2; a1++){	
	p[a1] = std::sqrt(KE[a1]*(KE[a1] + 2.0*malpha)); 
	E[a1] = KE[a1]+malpha;
	alpha_par1.SetPxPyPzE(p[a1]*sin(theta[a1])*cos(phi[a1]), p[a1]*sin(theta[a1])*sin(phi[a1]), p[a1]*cos(theta[a1]), E[a1]);

if(a1==0){ //AAP
	a2 = 1;
	pro = 2;
} else if(a1==1){ //APA
	a2 = 2;
	pro = 1;
}
//alpha2
	p[a2] = std::sqrt(KE[a2]*(KE[a2] + 2.0*malpha)); 
	E[a2] = KE[a2]+malpha;
	alpha_par2.SetPxPyPzE(p[a2]*sin(theta[a2])*cos(phi[a2]), p[a2]*sin(theta[a2])*sin(phi[a2]), p[a2]*cos(theta[a2]), E[a2]);
//proton
	p[pro] = std::sqrt(KE[pro]*(KE[pro] + 2.0*mproton));
	E[pro] = KE[pro]+mproton;
	proton.SetPxPyPzE(p[pro]*sin(theta[pro])*cos(phi[pro]), p[pro]*sin(theta[pro])*sin(phi[pro]), p[pro]*cos(theta[pro]), E[pro]);

B9_A = alpha_par1 + alpha_par2 + proton;
imm_exEn_A = B9_A.M() - m9B;

if(a1==0 && a2==1){
	configAAP->Fill(imm_exEn_A);
	IMMexE->Fill(imm_exEn_A);
}
else if(a1==1 && a2==2){
	configAPA->Fill(imm_exEn_A);
	IMMexE->Fill(imm_exEn_A);
}
}

//first particle proton
pro=0, a1=1, a2=2;
//proton
	p[pro] = std::sqrt(KE[pro]*(KE[pro] + 2.0*mproton));
	E[pro] = KE[pro]+mproton;
	proton_par1.SetPxPyPzE(p[pro]*sin(theta[pro])*cos(phi[pro]), p[pro]*sin(theta[pro])*sin(phi[pro]), p[pro]*cos(theta[pro]), E[pro]);
//alpha1
	p[a1] = std::sqrt(KE[a1]*(KE[a1] + 2.0*malpha)); 
	E[a1] = KE[a1]+malpha;
	alpha1_par2.SetPxPyPzE(p[a1]*sin(theta[a1])*cos(phi[a1]), p[a1]*sin(theta[a1])*sin(phi[a1]), p[a1]*cos(theta[a1]), E[a1]);
//alpha2
	p[a2] = std::sqrt(KE[a2]*(KE[a2] + 2.0*malpha)); 
	E[a2] = KE[a2]+malpha;
	alpha2_par3.SetPxPyPzE(p[a2]*sin(theta[a2])*cos(phi[a2]), p[a2]*sin(theta[a2])*sin(phi[a2]), p[a2]*cos(theta[a2]), E[a2]);

B9_P = alpha1_par2 + alpha2_par3 + proton_par1;
imm_exEn_P = B9_P.M() - m9B;

configPAA->Fill(imm_exEn_P);
IMMexE->Fill(imm_exEn_P);
}
}

datafile_3par.close();
//writes plots to file
	IMMplots->cd();
	configAAP->Write();
	configAPA->Write();
	configPAA->Write();
	IMMexE->Write();
	
	IMMplots->Close();
}

/***********************************************************************/

void boron::RunBoronMMM(){
//MMM for 1 particle events

TFile *plotsfile = new TFile ("/Users/rmshaffer/RMSRecon_v2/plots/B9_MMM_2alphaTest.root","recreate");
TH1F *alphas = new TH1F("AlphasDetected","9B_MMM",550,-1,10);

    //Kinematic constants
    double C = 299792458; //meters/second
    double QBRHO2P = 1.0E-9*C; //converts QBrho to momentum (cm*kG -> MeV)

    double mproton = MASS.FindMass(1,1);
    double m3He = MASS.FindMass(2,3);
    double malpha = MASS.FindMass(2,4);
    double m10B = MASS.FindMass(5,10);
    double m9B = MASS.FindMass(5,9);

 	double_t E_a1, E_a2;
 	double_t theta_a1, theta_a2;
 	double_t phi_a1, phi_a2;
	double_t xavg;

	double_t pAlpha[2];
	double_t EAlpha[2];

	double_t mmm_exEn_alphas;

TLorentzVector rxnAlpha, target, beam, b9_alpha1, b9_alpha2, pro_calc, boron9_2alphas;

 	std::vector<int> Zt = {5};
 	std::vector<int> At = {10};
 	std::vector<int> Stoich = {1};
 	targetEloss.SetTargetComponents(Zt, At, Stoich);
	target.SetPxPyPzE(0.,0.,0., m10B);
	
	double beamKE = 7.5; //MeV 
	double beamKErxn = beamKE - targetEloss.GetEnergyLoss(2,3,beamKE,25);
	double beamP = std::sqrt(beamKErxn*(beamKErxn + 2.0*m3He));
	double beamE = beamKErxn + m3He;
	beam.SetPxPyPzE(0.,0.,beamP,beamE);

std::ifstream datafile_2par;
datafile_2par.open("./etc/9B_2par.txt");
while(datafile_2par >> E_a1 >> theta_a1 >> phi_a1 >> E_a2 >> theta_a2 >> phi_a2 >> xavg){
double_t KE[2]={E_a1/1000,E_a2/1000};
double_t Theta[2]={theta_a1*TMath::Pi()/180,theta_a2*TMath::Pi()/180};
double_t Phi[2]={phi_a1*TMath::Pi()/180,phi_a2*TMath::Pi()/180};	

	double rho = 79.627 + 0.03741*xavg;
	double spsTheta = 20*TMath::Pi()/180; //radians
	double B = 7.30142; //kG	
	double rxnAlphaP = rho * 2.0 * B * QBRHO2P; //QBRHO2P has units of MeV/(kG*cm) taken from Yale analysis, tested in SPANC
	double rxnAlphaE = std::sqrt(rxnAlphaP*rxnAlphaP + malpha*malpha);
	//GetReverseEnergyLoss(Zprojectile, Aprojectile, KEprojectile (MeV), thickness (ug/cm^2))
	//Calculates the energy loss backwards through the target (tested for SPANC)
	double rxnAlphaKE_atrxn = (rxnAlphaE - malpha) + targetEloss.GetReverseEnergyLoss(2,4,rxnAlphaE - malpha, 25);
	double rxnAlphaP_atrxn = std::sqrt(rxnAlphaKE_atrxn*(rxnAlphaKE_atrxn + 2.0*malpha));
	double rxnAlphaE_atrxn = rxnAlphaKE_atrxn + malpha;
rxnAlpha.SetPxPyPzE(rxnAlphaP_atrxn*sin(spsTheta), 0., rxnAlphaP_atrxn*cos(spsTheta), rxnAlphaE_atrxn);

//2 alphas detected, proton calculated
//alpha 1
	double a1_KEatrxn = KE[0] + targetEloss.GetReverseEnergyLoss(2,4,KE[0],25);
	pAlpha[0] = std::sqrt(a1_KEatrxn*(a1_KEatrxn + 2.0*malpha));
	EAlpha[0] = a1_KEatrxn + malpha;

	b9_alpha1.SetPxPyPzE(pAlpha[0]*sin(Theta[0])*cos(Phi[0]), pAlpha[0]*sin(Theta[0])*sin(Phi[0]), pAlpha[0]*cos(Theta[0]), EAlpha[0]);

//alpha 2
	double a2_KEatrxn = KE[1] + targetEloss.GetReverseEnergyLoss(2,4,KE[1],25);
	pAlpha[1] = std::sqrt(a2_KEatrxn*(a2_KEatrxn + 2.0*malpha));
	EAlpha[1] = a2_KEatrxn + malpha;

	b9_alpha1.SetPxPyPzE(pAlpha[1]*sin(Theta[1])*cos(Phi[1]), pAlpha[1]*sin(Theta[1])*sin(Phi[1]), pAlpha[1]*cos(Theta[1]), EAlpha[1]);

	pro_calc = beam + target - b9_alpha1 - b9_alpha2;
	boron9_2alphas = pro_calc + b9_alpha1 + b9_alpha2;
	mmm_exEn_alphas = boron9_2alphas.M() - m9B;

	alphas->Fill(mmm_exEn_alphas);
}

datafile_2par.close();

plotsfile->cd();
alphas->Write();
plotsfile->Close();

}