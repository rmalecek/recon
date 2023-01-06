#ifndef LITHIUM_H
#define LITHIUM_H

#include <string>
#include <TROOT.h>
#include <TFile.h>
#include "TMath.h"
#include "Eloss_Tables.h"
#include "EnergyLoss.h"
#include "MassLookup.h"

class lithium {
public:

	void RunLithiumIMM();
	void RunLithiumMMM();
	
private:
    EnergyLoss targetEloss;

    /*Kinematic constants*/
    const double C = 299792458;
    const double QBRHO2P = 1.0E-9*C; //converts QBrho to momentum (cm*kG -> MeV)
    const double DEG2RAD = TMath::Pi()/180.0; //converts degrees to radians
};

#endif
