#ifndef BORON_H
#define BORON_H

#include <string>
#include <TROOT.h>
#include <TFile.h>
#include "Eloss_Tables.h"
#include "EnergyLoss.h"
#include "MassLookup.h"

class boron {
public:

	void RunBoronIMM();
	void RunBoronMMM();

private:

    EnergyLoss targetEloss;

};

#endif
