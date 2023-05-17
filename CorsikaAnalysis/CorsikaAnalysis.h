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


// FIXME!! Need to port every particle PDGID...
// Currently working only for long-lived particles which can arrive see-level
int getPDGID(const crsIO::TParticle* aPtc) {
	int ID = aPtc->ParticleID;
	if 		(ID == 1)   return    22; // photon
	else if (ID == 2)   return   -11; // e+
	else if (ID == 3)   return    11; // e-
	else if (ID == 5)   return   -13; // mu+
	else if (ID == 6)   return    13; // mu-
	else if (ID == 7)   return   111; // pi0
	else if (ID == 8)   return   211; // pi+
	else if (ID == 9)   return  -211; // pi-
	else if (ID == 10)  return   130; // K0_L
	else if (ID == 11)  return   321; // K+
	else if (ID == 12)  return  -321; // K-
	else if (ID == 13)  return  2112; // n
	else if (ID == 14)  return  2212; // p
	else if (ID == 15)  return -2212; // anti p
	else if (ID == 25)  return -2112; // anti n
	else if (ID == 66)  return    12; // nu_e
	else if (ID == 67)  return   -12; // anti nu_e
	else if (ID == 68)  return    14; // nu_mu
	else if (ID == 69)  return   -14; // anti nu_mu
	else if (ID == 133) return    16; // nu_tau
	else if (ID == 134) return   -16; // anti nu_tau
	else return 22; // FIXME!! Need to port every particle PDGID...
}

TFile* getInputFile(std::string runNum) {
  std::stringstream stream;
	stream << std::setw(6) << std::setfill('0') << runNum;
	TString fileName = (TString) ("../../run/out/DAT" + stream.str() +".root"); // FIX HERE!! Provide proper input CORSIKA output file path
	std::cout << "Reading CORSIKA file : " << fileName << std::endl;
	return TFile::Open(fileName, "Read");
}
