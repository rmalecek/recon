#ifndef FPPLOTS_H
#define FPPLOTS_H

#include <string>
#include <TROOT.h>
#include <TFile.h>
#include "Eloss_Tables.h"
#include "EnergyLoss.h"
#include "MassLookup.h"

class focalplane {
public:

	void RunLiFPplots();
	void RunBorFPplots();

private:

    EnergyLoss targetEloss;

};

#endif
