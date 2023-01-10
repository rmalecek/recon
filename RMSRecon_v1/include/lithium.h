#ifndef LITHIUM_H
#define LITHIUM_H

#include <string>
#include <TROOT.h>
#include <TFile.h>
#include "TMath.h"
#include "Eloss_Tables.h"
#include "EnergyLoss.h"
#include "MassLookup.h"
#include "SabreMap.h"
#include "SabreDetector.h"
#include "Vec3.h"

class lithium {
public:

	void RunLithiumIMM();
	void RunLithiumMMM();
 
 	std::vector<Mask::Vec3> GetSabreCoords(const Mask::SabreDetector& sabreHit);
    void GetFilledData(double* x, double* y, double* z);
	
private:
    Mask::SabreDetector* SabreArray[5];
    double filled_x[5*8*16];
    double filled_y[5*8*16];
    double filled_z[5*8*16]; //Convention: channel is det#*128+ring#*8+wedge#
    /***SABRE detector info***/
    /*distances in meters, angles in degrees*/
    const double INNER_R = 0.0326;
    const double OUTER_R = 0.1351;
    const double TILT = 40.0;
    //const double DIST_2_TARG = 0.14549;
    const double DIST_2_TARG = -0.1245;
    const double PHI_COVERAGE = 54.4; //delta phi for each det
    const double PHI2 = 234.0; //center phi values for each det in array
    const double PHI3 = 162.0; //# is equal to detID in channel map
    const double PHI0 = 306.0;
    const double PHI1 = 18.0;
    const double PHI4 = 90.0;

    SabreMap smap;

    bool IllegalMap;

    EnergyLoss targetEloss;

    /*Kinematic constants*/
    const double C = 299792458;
    const double QBRHO2P = 1.0E-9*C; //converts QBrho to momentum (cm*kG -> MeV)
    const double DEG2RAD = TMath::Pi()/180.0; //converts degrees to radians
};

#endif
