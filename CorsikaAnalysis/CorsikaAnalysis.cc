#include "CorsikaAnalysis.h"

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

int main(int argc, char *argv[]){
	// Argument check
	if (argc != 2) {
		std::cout << "[Error] Wrong number of arguments" << std::endl;
		std::cout << "Usage : ./test.exe RunNum " << std::endl;
		return 0;
	}
	
	// Load CORSIKA file
	std::string runNum = argv[1];
	TFile* aFile = getInputFile(runNum);

	// Load shower info (crsIO::TShower class, which contains information of particles in same shower)
	TTree* aTree = (TTree*) aFile->Get("sim");
	crsIO::TShower* pShower = new crsIO::TShower();
	aTree->SetBranchAddress("shower.", &pShower);
	std::cout << "Total Event : " << aTree->GetEntries() << std::endl << std::endl;

	// Histogram declaration
	TH1D* ptc_E_total  = new TH1D("Ptc_energy", "Ptc_energy;GeV;Ptc", 2000, 0, 2000);
	TH1D* ptc_ID_total = new TH1D("Ptc_PDGID", "Ptc_PDGID;ID;Ptc", 195, 1, 196); // CORSIKA ID : 1 ~ 196, different from PDGID
	TH1D* ptc_T_total  = new TH1D("Ptc_time", "Ptc_time;ns;Ptc/10ns", 100000, 0, 1000000);
	TH2D* ptc_XY_total = new TH2D("Ptc_XY", "Ptc_XY;m;m", 4000, -20000, 20000, 4000, -20000, 20000);
	// Container for histogram per shower
	std::vector<TH1D*> ptc_E;
	std::vector<TH1D*> ptc_ID;
	std::vector<TH1D*> ptc_T;
	std::vector<TGraph*> ptc_XY;
	for (int iEvt = 0; iEvt < aTree->GetEntries(); iEvt++) {
		ptc_E. push_back( new TH1D(Form("Ptc_energy_%d", iEvt), Form("Ptc_energy_%d;GeV;Ptc", iEvt), 2000, 0, 2000) );
		ptc_ID.push_back( new TH1D(Form("Ptc_PDGID_%d", iEvt), Form("Ptc_PDGID_%d;ID;Ptc", iEvt), 195, 1, 196) );
		ptc_T. push_back( new TH1D(Form("Ptc_time_%d", iEvt), Form("Ptc_time_%d;ns;Ptc/10ns", iEvt), 100000, 0, 1000000) );
		ptc_XY.push_back( new TGraph() );
		ptc_XY.at(iEvt)->SetNameTitle( Form("Ptc_XY_%d" ,iEvt), Form("Ptc_XY_%d;cm;cm" ,iEvt) );
	}

	// Shower loop (== Event loop)
	int nPtc = 0;
	for (int iEvt =0; iEvt < aTree->GetEntries(); iEvt++) {
		aTree->GetEntry(iEvt);
		// Get particle data (in TClonesArray with class crsIO::TParticle)
		TClonesArray aParticles = pShower->GetParticles();
		std::cout << "Evt # : " << iEvt << " total # of ptc : " << aParticles.GetEntries() << std::endl;

		//Loop on particles in same shower
		crsIO::TParticle* ptc;
		for (int idx = 0; idx < aParticles.GetEntries(); idx++) {
			ptc = (crsIO::TParticle*) aParticles.At(idx);
			ptc_XY.at(iEvt)->SetPoint(idx, ptc->x, ptc->y); ptc_XY_total->Fill(ptc->x, ptc->y);
			ptc_E.at(iEvt)->Fill( calculateE(ptc) );  ptc_E_total->Fill( calculateE(ptc) );
			ptc_ID.at(iEvt)->Fill( ptc->ParticleID ); ptc_ID_total->Fill( ptc->ParticleID );
			ptc_T.at(iEvt)->Fill( ptc->Time );		 ptc_T_total->Fill( ptc->Time );
			nPtc++;
		}
	}
	std::cout << "Total number of particles per shower   : " << nPtc << std::endl;
	std::cout << "Average number of particles per shower : " << nPtc / aTree->GetEntries() << std::endl;


	// Open output root file
	std::stringstream stream;
	stream <<  std::setw(6) << std::setfill('0') << runNum;
	TFile* outputFile = TFile::Open( (TString)("./Hist_CORSIKA_" + stream.str() + ".root"), "RECREATE");
	outputFile->cd();
	ptc_XY_total->Write();
	ptc_E_total ->Write();
	ptc_ID_total->Write();
	ptc_T_total ->Write();
	for (int iEvt = 0; iEvt < aTree->GetEntries(); iEvt++) {
		ptc_XY.at(iEvt)->Write();
		ptc_E.at(iEvt) ->Write(); 
		ptc_ID.at(iEvt)->Write();
		ptc_T.at(iEvt) ->Write();
	}
	outputFile->Close();

	// Exiting...
	ptc_XY.clear();
	ptc_E.clear();
	ptc_ID.clear();
	ptc_T.clear();
	delete ptc_XY_total;
	delete ptc_E_total;
	delete ptc_ID_total;
	delete ptc_T_total;
	delete aTree;
	delete pShower;
	aFile->Close();

	return 0;
}
