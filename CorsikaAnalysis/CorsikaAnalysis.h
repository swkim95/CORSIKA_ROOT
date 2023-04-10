#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TString.h"

#include <crsIO/TRun.h>
#include <crsIO/TShower.h>
#include <crsIO/TParticle.h>
#include <crsIO/TCherenkov.h>
#include <crsIO/TLongitudinal.h>

#include <iostream>
#include <sstream> 
#include <iomanip> 
#include <vector>
#include <cmath>

/*
	crsIO::TParticle class variables

	int CorsikaID;
    int ParticleID;
    int ObservationLevel;
    int HadronicGeneration;
    
    double Px;
    double Py;
    double Pz;
    
    double x;
    double y;
    
    double Time;
    double Weight;
*/

// Calculate Energy of particle, using mass reference from Corsika guidebook
double calculateE(crsIO::TParticle* ptc) {
	int ID = ptc->ParticleID;
  	double Px = ptc->Px;
  	double Py = ptc->Py;
  	double Pz = ptc->Pz;
	if 		(ID == 1)  return std::sqrt(Px*Px + Py*Py + Pz*Pz); // photon
	else if (ID == 2)  return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.000511*0.000511); // e+
	else if (ID == 3)  return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.000511*0.000511); // e-
	else if (ID == 5)  return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.105658*0.105658); // mu+
	else if (ID == 6)  return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.105658*0.105658); // mu-
	else if (ID == 7)  return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.134977*0.134977); // pi0
	else if (ID == 8)  return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.139570*0.139570); // pi+
	else if (ID == 9)  return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.139570*0.139570); // pi-
	else if (ID == 10) return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.497610*0.497610); // K0_L
	else if (ID == 11) return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.493677*0.493677); // K+
	else if (ID == 12) return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.493677*0.493677); // K-
	else if (ID == 13) return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.939565*0.939565); // n
	else if (ID == 14) return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.938272*0.938272); // p
	else if (ID == 15) return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.938272*0.938272); // anti p
	else if (ID == 25) return std::sqrt(Px*Px + Py*Py + Pz*Pz + 0.939565*0.939565); // anti n
	else 			   return std::sqrt(Px*Px + Py*Py + Pz*Pz); //probably neutrinos
}

TFile* getInputFile(std::string runNum) {
  std::stringstream stream;
	stream << std::setw(6) << std::setfill('0') << runNum;
	TString fileName = (TString) ("../run/out/DAT" + stream.str() +".root");
	std::cout << "Reading CORSIKA file : " << fileName << std::endl;
	return TFile::Open(fileName, "Read");
}
