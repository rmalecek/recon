#include "lithium.h"
#include "TH1F.h"
#include "TFile.h"
#include "EnergyLoss.h"
#include "MassLookup.h"
#include <iostream>
#include <TLorentzVector.h>
#include <vector>

/*
By RMS 7/18/22
Invariant mass and missing mass calculations for Li6 done with 
1 and 2 particle text files output from printData Plotter in GWMEVB code

MMM missing particle = target + beam - rxnalpha - detected particle
*/

void lithium::RunLithiumIMM(){ //Invariant Masss Method for 2 particle events

    double massdeuteron = MASS.FindMass(1,2); 
    double massalpha = MASS.FindMass(2,4);
    double mass6Li = MASS.FindMass(3,6);

//creates file plots will be written to
TFile *plotsfile = new TFile ("/Users/rmshaffer/RMSRecon_v0/plots/Li6.root","recreate");

//creates plots to fill
TH1F *configAD = new TH1F("ADconfig","6Li_IMMExEn",550,-1,10);
TH1F *configDA = new TH1F("DAconfig","6Li_IMMExEn",550,-1,10);
TH1F *totIMM = new TH1F("Li6_IMMExEn","6Li_all2par",550,-1,10);
//TH1F *totHits = new TH1F("Li6_allExEn","6Li_allHits",550,-1,10);

	double_t E1_2par;
	double_t theta1_2par;
	double_t phi1_2par;	
	double_t E2_2par;
	double_t theta2_2par;
	double_t phi2_2par;
	double_t xavg;

	double_t E_AD[2];
	double_t p_AD[2];
	double_t E_DA[2];
	double_t p_DA[2];

	double_t imm_exEn_AD;
	double_t imm_exEn_DA;

TLorentzVector alpha1, alpha2, deut1, deut2, lith6_AD, lith6_DA;

std::ifstream datafile_2par;
datafile_2par.open("./etc/LiF_2par.txt");

//energies in keV and angles in degrees
while(datafile_2par >> E1_2par >> theta1_2par >> phi1_2par >> E2_2par >> theta2_2par >> phi2_2par >> xavg){
if(xavg<-8 && xavg>-22){
double_t KE[2]={E1_2par/1000,E2_2par/1000}; //convert energies to MeV
double_t theta[2]={theta1_2par*(TMath::Pi()/180),theta2_2par*(TMath::Pi()/180.0)}; //convert degrees to radians
double_t phi[2]={phi1_2par*(TMath::Pi()/180.0),phi2_2par*(TMath::Pi()/180.0)};
	
if(KE[1]>KE[0]){
//alpha as particle 1
	p_AD[0] = std::sqrt(KE[0]*(KE[0] + 2.0*massalpha)); 
	E_AD[0] = KE[0]+massalpha;
	alpha1.SetPxPyPzE(p_AD[0]*sin(theta[0])*cos(phi[0]), p_AD[0]*sin(theta[0])*sin(phi[0]), p_AD[0]*cos(theta[0]), E_AD[0]);

//deuteron as particle 2
	p_AD[1] = std::sqrt(KE[1]*(KE[1] + 2.0*massdeuteron));
	E_AD[1] = KE[1]+massdeuteron;
	deut2.SetPxPyPzE(p_AD[1]*sin(theta[1])*cos(phi[1]), p_AD[1]*sin(theta[1])*sin(phi[1]), p_AD[1]*cos(theta[1]), E_AD[1]);

lith6_AD = alpha1 + deut2;
imm_exEn_AD = lith6_AD.M() - mass6Li;

configAD->Fill(imm_exEn_AD);
totIMM->Fill(imm_exEn_AD);
} 

else if(KE[0]>KE[1]){
//deuteron as particle 1
	p_DA[0] = std::sqrt(KE[0]*(KE[0] + 2.0*massdeuteron));
	E_DA[0] = KE[0]+massdeuteron;
	deut1.SetPxPyPzE(p_DA[0]*sin(theta[0])*cos(phi[0]), p_DA[0]*sin(theta[0])*sin(phi[0]), p_DA[0]*cos(theta[0]), E_DA[0]);

//alpha as particle 2
	p_DA[1] = std::sqrt(KE[1]*(KE[1] + 2.0*massalpha)); 
	E_DA[1] = KE[1]+massalpha;
	alpha2.SetPxPyPzE(p_DA[1]*sin(theta[1])*cos(phi[1]), p_DA[1]*sin(theta[1])*sin(phi[1]), p_DA[1]*cos(theta[1]), E_DA[1]);

lith6_DA = alpha2 + deut1;
imm_exEn_DA = lith6_DA.M() - mass6Li;

configDA->Fill(imm_exEn_DA);
totIMM->Fill(imm_exEn_DA);
}
}
}

datafile_2par.close();
//Write plots to file
plotsfile->cd();

configAD->Write();
configDA->Write();
totIMM->Write();

plotsfile->Close();
}

/***********************************************************************/

void lithium::RunLithiumMMM(){ //Missing Mass Method for 1 particle events
//Assuming decay alpha is detected and deuteron is missing

TFile *plotsfile = new TFile ("/Users/rmshaffer/RMSRecon_v0/plots/Li6.root","update");
TH1F *alphapar = new TH1F("AlphaDetected","6Li_MMM",550,-1,10);
TH1F *deutpar = new TH1F("DeuteronDetected","6Li_MMM",550,-1,10);
TH1F *totMMM = new TH1F("Li6_MMMExEn","6Li_all1par",550,-1,10);
 	    
 	double massdeuteron = MASS.FindMass(1,2); 
    double massalpha = MASS.FindMass(2,4);
    double mass6Li = MASS.FindMass(3,6);
    double mass7Li = MASS.FindMass(3,7);
	double mass3He = MASS.FindMass(2,3);

    double C = 299792458; //meters/second
    double QBRHO2P = 1.0E-9*C; //converts QBrho to momentum (cm*kG -> MeV)

 	double_t E_1par;
 	double_t theta_1par;
 	double_t phi_1par;
	double_t xavg;

	double_t p1par_deut, p1par_alpha;
	double_t E1par_deut, E1par_alpha;

	double_t mmm_exEn_alpha;
	double_t mmm_exEn_deut;

TLorentzVector rxnAlpha, target, beam, lith6_alpha, lith6_deut, deut_calc, dAlpha_obs, dAlpha_calc, deut_obs;

	target.SetPxPyPzE(0.,0.,0., mass7Li);
	
	double beamKE = 7.5; //MeV 
	double beamKErxn = beamKE - targetEloss.GetEnergyLoss(2,3,beamKE,25);
	double beamP = std::sqrt(beamKErxn*(beamKErxn + 2.0*mass3He));
	double beamE = beamKErxn + mass3He;
	beam.SetPxPyPzE(0.,0.,beamP,beamE);

std::ifstream datafile_1par;
datafile_1par.open("./etc/LiF_1par.txt");
while(datafile_1par >> E_1par >> theta_1par >> phi_1par >> xavg){
if(xavg<-8 && xavg>-22){
double_t v1par[4]={E_1par/1000,theta_1par*TMath::Pi()/180,phi_1par*TMath::Pi()/180,xavg};	

	double rho = 79.627 + 0.03741*v1par[3]; //FP calibration from GWM SabreRecon used to FPcal code
	double spsTheta = 20*TMath::Pi()/180; //radians
	double B = 7.30142; //kG	
	double rxnAlphaP = rho * 2.0 * B * QBRHO2P; //QBRHO2P has units of MeV/(kG*cm) taken from Yale analysis, FPcaled in SPANC
	double rxnAlphaE = std::sqrt(rxnAlphaP*rxnAlphaP + massalpha*massalpha);
	//GetReverseEnergyLoss(Zprojectile, Aprojectile, KEprojectile (MeV), thickness (ug/cm^2))
	//Calculates the energy loss backwards through the target (FPcaled for SPANC)
	//double CEloss = (rxnAlphaE - massalpha) + targetEloss.GetReverseEnergyLoss(2,4,rxnAlphaE - massalpha,20);

	std::vector<int> Zt_LiF = {3,9};
 	std::vector<int> At_LiF = {7,18};
 	std::vector<int> Stoich_LiF = {1,1};
 	targetEloss.SetTargetComponents(Zt_LiF, At_LiF, Stoich_LiF);
 	
	double rxnAlphaKE_atrxn = (rxnAlphaE - massalpha) + targetEloss.GetReverseEnergyLoss(2,4,rxnAlphaE - massalpha, 25);
	double rxnAlphaP_atrxn = std::sqrt(rxnAlphaKE_atrxn*(rxnAlphaKE_atrxn + 2.0*massalpha));
	double rxnAlphaE_atrxn = rxnAlphaKE_atrxn + massalpha;
	rxnAlpha.SetPxPyPzE(rxnAlphaP_atrxn*sin(spsTheta), 0., rxnAlphaP_atrxn*cos(spsTheta), rxnAlphaE_atrxn);

//decay alpha detected, calculate missing deuteron	
	double da_KEatrxn = v1par[0] + targetEloss.GetReverseEnergyLoss(2,4,v1par[0],25);
	p1par_alpha = std::sqrt(da_KEatrxn*(da_KEatrxn + 2.0*massalpha));
	E1par_alpha = da_KEatrxn + massalpha;
	dAlpha_obs.SetPxPyPzE(p1par_alpha*sin(v1par[1])*cos(v1par[2]), p1par_alpha*sin(v1par[1])*sin(v1par[2]), p1par_alpha*cos(v1par[1]), E1par_alpha);
	
	deut_calc = beam + target - rxnAlpha - dAlpha_obs;

	lith6_alpha = dAlpha_obs + deut_calc;

	mmm_exEn_alpha = lith6_alpha.M() - mass6Li;

	alphapar->Fill(mmm_exEn_alpha);	
	totMMM->Fill(mmm_exEn_alpha);

 //deuteron detected, calculate missing decay alpha
	double d_KEatrxn = v1par[0] + targetEloss.GetReverseEnergyLoss(1,2,v1par[0],25);
	p1par_deut = std::sqrt(d_KEatrxn*(d_KEatrxn + 2.0*massdeuteron));
	E1par_deut = d_KEatrxn + massdeuteron;
	deut_obs.SetPxPyPzE(p1par_deut*sin(v1par[1])*cos(v1par[2]), p1par_deut*sin(v1par[1])*sin(v1par[2]), p1par_deut*cos(v1par[1]), E1par_deut);

	dAlpha_calc = beam + target - rxnAlpha - deut_obs;
	lith6_deut = dAlpha_calc + deut_obs;

	mmm_exEn_deut = lith6_deut.M() - mass6Li;

	deutpar->Fill(mmm_exEn_deut);
	totMMM->Fill(mmm_exEn_deut);
}
}
datafile_1par.close();
plotsfile->cd();
alphapar->Write();
deutpar->Write();
totMMM->Write();
plotsfile->Close();
}	
