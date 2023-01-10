#include <iostream>
#include <string>
#include <fstream>
#include "boron.h"
#include "lithium.h"
#include "FPplots.h"

//RMSRecon Version 1. Updating RMSRecon v0. Trying to implement SABRE reconstruction from GWMSabreRecon code 

int main(int argc, char** argv) {
if(argc != 2){
	std::cerr<<"please specify target nucleus; lithium or boron"<<std::endl;
	return 1;
}

std::string target = argv[1];
std::string sabrefile = "./ect/ChannelMap_Feb2021_SABRE.txt";

if(target == "boron"){
	boron B9;
	std::cout<<"Running invariant mass and missing mass calculations for 9B"<<std::endl;
    B9.RunBoronIMM();
	B9.RunBoronMMM();
}

else if(target == "lithium"){
	lithium Li6;
	std::cout<<"Running invariant mass and missing mass calculations for 6Li"<<std::endl;
	Li6.RunLithiumIMM();
	Li6.RunLithiumMMM();
}

else if(target == "calibrate"){
	focalplane FP;
	std::cout<<"Calibrating focal plane energies"<<std::endl;
	FP.RunLiFPplots();
	FP.RunBorFPplots();
} 

return 0;

}
