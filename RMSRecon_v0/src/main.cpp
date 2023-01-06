#include <iostream>
#include <string>
#include <fstream>
#include "boron.h"
#include "lithium.h"

int main(int argc, char** argv) {
if(argc != 2){
	std::cerr<<"please specify target nucleus; lithium or boron"<<std::endl;
	return 1;
}

std::string target = argv[1];

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

return 0;

}
