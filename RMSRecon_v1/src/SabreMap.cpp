#include "SabreMap.h"

SabreMap::SabreMap() :
	validFlag(false)
{
}

SabreMap::SabreMap(std::string filename) :
	validFlag(false)
{
	FillMap(filename);
}

SabreMap::~SabreMap() {}

void SabreMap::FillMap(std::string filename) {
	std::ifstream input(filename);
	std::string junk;
	int gchan, channel, det;
	if(!input.is_open()) {
		std::cerr<<"You didnt put the right fuckin file in"<<std::endl;
	}

	std::getline(input, junk);
	std::getline(input, junk);
	
	sabrechan this_schan;
	std::string key;
	while(input>>gchan) {
		input>>det>>junk>>channel;
		if(junk == "SABREWEDGE") {
			this_schan.ringFlag = false;
			this_schan.wedgeFlag = true;
			key = std::to_string(det) + "WEDGE" + std::to_string(channel);
		} else if(junk == "SABRERING") {
			this_schan.ringFlag = true;
			this_schan.wedgeFlag = false;
			key = std::to_string(det) + "RING" + std::to_string(channel);
		} else {
			continue;
		}
		this_schan.detID = det;
		this_schan.channel = channel;
		smap[gchan] = this_schan;
		gmap[key] = gchan;
	}
	validFlag = true;
}

SabreMap::GIter SabreMap::GetGlobalChannel(int det, int chan, bool ringFlag) {
	std::string key;
	if(ringFlag) key = std::to_string(det) + "RING" + std::to_string(chan);
	else key = std::to_string(det) + "WEDGE" + std::to_string(chan);

	return gmap.find(key);
}
