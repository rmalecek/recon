/*SabreMap.h
 *Creates an unordered_map of sabre channels from an input file.
 *Based on work by S. Balak, K. Macon, and E. Good from LSU.
 *
 *Gordon M. Oct. 2019
 */
#ifndef SABRE_MAP_H
#define SABRE_MAP_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

struct sabrechan {
	int detID;
	bool ringFlag;
	bool wedgeFlag;
	int channel;
};

class SabreMap {
public:
	typedef std::unordered_map<int, sabrechan>::iterator SIter;
	typedef std::unordered_map<std::string, int>::iterator GIter;
	SabreMap();
	SabreMap(std::string filename);
	~SabreMap();

	bool IsValid() { return validFlag; };
	SIter GetSabreInfo(int gchan) { return smap.find(gchan); };
	GIter GetGlobalChannel(int det, int chan, bool ringFlag);
	SIter GetSMapEnd() { return smap.end(); };
	GIter GetGMapEnd() { return gmap.end(); };

private:
	void FillMap(std::string filename);
	bool validFlag;
	std::unordered_map<int, sabrechan> smap;
	std::unordered_map<std::string, int> gmap;
};

#endif
